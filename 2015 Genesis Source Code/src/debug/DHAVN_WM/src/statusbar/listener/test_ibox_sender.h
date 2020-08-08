#ifndef	TEST_IBOX_SENDER_H_
#define	TEST_IBOX_SENDER_H_

#include <QObject>
#include <QDBusAbstractAdaptor>
#include <QDBusMessage>
#include <QDBusVariant>
#include <QDBusReply>

class TestIBoxSender : public QDBusAbstractAdaptor {
	Q_OBJECT
	Q_CLASSINFO( "D-Bus Interface", "com.lge.car.iboxtest.osd")

public:
	explicit TestIBoxSender(QObject * parent = 0);

public slots:
	void displayOSD(const QDBusMessage & msg);

signals:
	void msgArrived(uchar, uchar, uchar, uchar, uchar, QString);
};

#endif
