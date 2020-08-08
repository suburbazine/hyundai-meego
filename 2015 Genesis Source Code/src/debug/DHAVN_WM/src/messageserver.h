#ifndef	MESSAGE_SERVER_H_
#define	MESSAGE_SERVER_H_

#include <QObject>
#include <QHash>

class QLocalSocket;
class QLocalServer;

class MessageServer : public QObject{
    Q_OBJECT
public:
    MessageServer(QObject * obj = 0);
    ~MessageServer();

    void	startServer();

private:
    QLocalServer * server;
    bool			started;
    bool			alreadySentAgree;
    //dmchoi_20131223 [ OSD 표시되는 구간에서 언어가 변경될 경우, OSD UI는 기존 언어 표시 형식을 따르고,
    //다음 번 표출 시에 변경된 언어에 맞게 UI를 변경하도록
    bool                        languageStatus;
    QString                     tmpString;
    //dmchoi_20131223 ]
    QList<QLocalSocket *> clientList;
#ifdef	TRACE_SENDER
    QHash<QLocalSocket *, QString> debugInfo;
#endif

    void			sendCurrentStatus(QLocalSocket *);

private slots:
    void	connectionIncome();
    void	readData();
    void	removeSock();

public slots:
    void	sendBroadcastMessage(const QString &);
    void	sendAgreed();
    void	sendBackAgreed();

signals:
    void	messageArrived(const QString &);
};

#endif
