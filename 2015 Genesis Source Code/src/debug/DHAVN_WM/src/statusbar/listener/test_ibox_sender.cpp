#include "test_ibox_sender.h"
#include "dhavnlog.h"

TestIBoxSender::TestIBoxSender(QObject * parent)
: QDBusAbstractAdaptor(parent){

}

void TestIBoxSender::displayOSD(const QDBusMessage & msg){
	const QList<QVariant> & args = msg.arguments();

	lprintf("Receive : %d", args.count());

	uchar target = (uchar)args.at(0).toUInt();
	uchar osdId = (uchar)args.at(1).toUInt();
	uchar osdControl = (uchar)args.at(2).toUInt();
	uchar osdMode = (uchar)args.at(3).toUInt();
	uchar textColor = (uchar)args.at(4).toUInt();
	QString text = args.at(5).toString();

	emit msgArrived(target, osdId, osdControl, osdMode, textColor, text);
}
