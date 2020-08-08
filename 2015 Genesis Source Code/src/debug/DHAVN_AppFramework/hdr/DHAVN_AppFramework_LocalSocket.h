#ifndef LocalSocketIpcClient_H
#define LocalSocketIpcClient_H

#include <QtNetwork/QLocalSocket>
#include "DHAVN_AppFramework_Logger_Def.h"

const QString KLocalSocketName( "/home/meego/.config/uish" );
const QString KCancel  ("UPGRADE_CANCEL");
const QString KFailure ("UPGRADE_FAILURE");
const QString KSuccess ("UPGRADE_SUCCESS");
const QString KRegister("UPGRADE_REGISTER");



class LocalSocketIpcClient : public QObject
{
    Q_OBJECT
public:
    LocalSocketIpcClient(QString remoteServername, QObject *parent = 0);
    ~LocalSocketIpcClient();

    enum Messagetype{
        MSG_STRING,
        MSG_BYARRY
    };

signals:

public slots:
    void send(QString message);
    void send(QByteArray message);

    void socket_connected();
    void socket_disconnected();

    void socket_readReady();
    void socket_error(QLocalSocket::LocalSocketError);

private:
    QLocalSocket*   m_socket;
    quint16         m_blockSize;
    QString         m_msgString;
    QByteArray      m_msgByteArray;
    QString         m_serverName;

    Messagetype     m_type;
};

#endif
