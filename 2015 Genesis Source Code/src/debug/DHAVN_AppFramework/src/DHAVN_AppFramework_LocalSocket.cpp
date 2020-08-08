
#include <DHAVN_AppFramework_LocalSocket.h>

LocalSocketIpcClient::LocalSocketIpcClient(QString remoteServername, QObject *parent) :
        QObject(parent), m_blockSize(0), m_msgString(), m_msgByteArray() {

    m_socket = new QLocalSocket(this);
    m_serverName = remoteServername;

    m_type = MSG_STRING;

    connect(m_socket, SIGNAL(connected()), this, SLOT(socket_connected()));
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(socket_disconnected()));

    connect(m_socket, SIGNAL(readyRead()), this, SLOT(socket_readReady()));
    connect(m_socket, SIGNAL(error(QLocalSocket::LocalSocketError)),
            this, SLOT(socket_error(QLocalSocket::LocalSocketError)));
}

LocalSocketIpcClient::~LocalSocketIpcClient() {
    m_socket->abort();
    delete m_socket;
    m_socket = NULL;
}

void LocalSocketIpcClient::send(QString message) {
    m_socket->abort();
    m_type = MSG_STRING;
    m_msgString = message;
    m_socket->connectToServer(m_serverName);
}

void LocalSocketIpcClient::send(QByteArray message){
    m_socket->abort();
    m_type = MSG_BYARRY;
    m_msgByteArray = message;
    m_socket->connectToServer(m_serverName);
}


void LocalSocketIpcClient::socket_connected(){
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    if ( m_type == MSG_STRING)
        out << m_msgString;
    else
        out << m_msgByteArray;

    out.device()->seek(0);
    m_socket->write(block);
    m_socket->flush();
}

void LocalSocketIpcClient::socket_disconnected() {
    Logger::Instance()->Log( "socket_disconnected" );
}


void LocalSocketIpcClient::socket_readReady() {
    Logger::Instance()->Log( "socket_readReady" );
}

void LocalSocketIpcClient::socket_error(QLocalSocket::LocalSocketError) {
    Logger::Instance()->Log( "socket_error" );
}


