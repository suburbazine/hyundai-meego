#ifndef	MESSAGE_EVENT_H_
#define	MESSAGE_EVENT_H_

#include <QObject>
#include "messageserver.h"

class MessageEvent : public QObject{
Q_OBJECT

public:
	MessageEvent(QObject * obj = 0);
	~MessageEvent();
	static QString GetModuleIdToString(int id);
	static QString GetModuleIdToPackageName(int id);
	static QString GetModulePackageNameToDisplay(const QString & packageName);

private:
	MessageServer * server;
	QString getModuleStatusToString(int status, const QString & msg);

private slots:
	void receiveEvent(int id, int progress, int status, const QString & msg);

signals:
	void messageProgress(const QString &, const QString &, int);
	void messageProgressRaw(int, int, int, const QString &);
};

#endif
