#ifndef	MESSAGE_SERVER_H_
#define	MESSAGE_SERVER_H_

#include <QObject>
#include <QSignalMapper>
#include <QLocalSocket>
#include <QLocalServer>

#define MAX_MODULE_COUNT	20

class MessageServer : public QObject{
Q_OBJECT
public:
	MessageServer(QObject * obj = 0);
	~MessageServer();

	void	startServer();

private:
	QLocalServer * server;
	QSignalMapper * mapper;
	bool			started;

private slots:
	void	connectionIncome();
	void	readData(QObject *);

signals:
	void	messageArrived(int, int, int, const QString &);

};

#endif
