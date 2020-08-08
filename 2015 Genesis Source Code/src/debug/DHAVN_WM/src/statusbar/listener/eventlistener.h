#ifndef	EVENT_LISTENER_H_
#define EVENT_LISTENER_H_

#include <QObject>

#include <QDBusConnection>
#include <QDBusMessage>

class EventListener : public QObject{
Q_OBJECT
public:
	EventListener(QObject * p = 0);

signals:
	void eventTriggered(const QString &);

private:
	void volumeChanged(qint8,qint8);
	QDBusMessage msg;

private slots:
	void handleAudioMessage(QDBusMessage);
	void beep();
	void handleCarIFMessage(uchar, uchar);
};

#endif
