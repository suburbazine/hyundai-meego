#ifndef	MESSAGE_PROCESS_H_
#define MESSAGE_PROCESS_H_

#include <QObject>
#include <QLocalSocket>
#include <QStringList>
#include "log_console.h"

class MessageProcess : public QObject{
	Q_OBJECT
public:
	bool writeMessage(const QString & msg);
	bool isRegistered() { return m_regist; }

	static MessageProcess * instance(const QString & aName = QString()){
		if ( applicationName == "UNKNOWN" && !aName.isEmpty())
			applicationName = aName;

		if ( applicationName == "UNKNOWN" && messageProcess == NULL)
			return NULL;

                if ( messageProcess == NULL){
                        lprintf("[StatusBar] create new messageprocess");
			messageProcess = new MessageProcess;
		}

		return messageProcess;
	}

	void registApp();

	bool isAgreed() { return m_agreed; }


private:
	MessageProcess(QObject * obj = 0);

	static MessageProcess * messageProcess;
	static QString 			applicationName;

	QLocalSocket * m_sock;

	bool connectToServer();

	bool m_regist;
	bool m_agreed;


private slots:
	void readData();
	void checkState(QLocalSocket::LocalSocketState);

signals:
	void statusCommandReceived(const QStringList &);
	void eventCommandReceived(const QStringList &);
	void agreed();
	void backToAgree();
};

#endif
