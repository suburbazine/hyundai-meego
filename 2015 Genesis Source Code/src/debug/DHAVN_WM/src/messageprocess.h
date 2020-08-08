#ifndef MESSAGE_PROCESS_H_
#define MESSAGE_PROCESS_H_

#include <QObject>
#include <QStringList>
#include "messagefilter.h"

class StatusBar;

class MessageProcess : public QObject{
Q_OBJECT
public:
	MessageProcess(QObject * obj = 0);
	void sendToMain(const QString & str);

private:
	void process_evtCommand(int, const QStringList &);

	QStringList commandQueue;
	QString		lastBluelinkRssi;

	QString		lastTime;

	bool		m_ignoreMode;

	MessageFilter m_messageFilter;

	bool isLCDOff;
	bool isPowerOff;
	bool requestLCDStatus;

	void statusCommand(const QStringList &);

public slots:
	void process(const QString &);

	void sendFrontHomeClick();
	void statusBarHide();

signals:
	void operationCommand(int, const QStringList &);
	void osdCommand(int, const QString &, const QStringList &, uchar);
	void beep();

	void osdLock(bool, bool);
        void screenSwap(bool);
        void screenClone(int);

private slots:
	void runListener();

signals:
	void languageChanged(int);
	void applicationRegistered(const QString &);
	void broadcastCommand(const QString &);

	void clearOSDAll();
	void clearFrontOSD();
};

#endif
