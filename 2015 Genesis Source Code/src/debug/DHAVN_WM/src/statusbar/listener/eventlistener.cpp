#include "eventlistener.h"
#include "dhavnlog.h"

EventListener::EventListener(QObject * obj)
: QObject(obj){
	msg = QDBusMessage::createMethodCall("com.lge.car.micom", "/", "com.lge.car.micom.audioif", "ProcessAudioControlMessage");
	QList<QVariant> args;
	args.append( (quint16) 0x0115);
	args.append( (quint8) 0x0);
	msg.setArguments(args);

	QDBusConnection::sessionBus().connect("com.lge.car.micom", "/", "com.lge.car.micom.audioif", "EventMessage", this, SLOT(handleAudioMessage(QDBusMessage)));
	QDBusConnection::sessionBus().connect("com.lge.car.micom", "/SysInfo", "com.lge.car.micom.sysinfo", "LgeDBus_EventCarIFStatus", this, SLOT(handleCarIFMessage(uchar, uchar)));
}

void EventListener::beep(){
	QDBusConnection::sessionBus().call(msg, QDBus::NoBlock);
}

void EventListener::handleAudioMessage(QDBusMessage msg){
	QList<QVariant> list;
	list = msg.arguments();

	if ( list.count() == 3)
		volumeChanged(list.at(1).toInt(), list.at(2).toInt());	
}

void EventListener::handleCarIFMessage(uchar type, uchar status){
	if ( type == 0x0B ){	// RSE Lock
		QString command = "@STA^RSE^";
		if ( status == 0x0){
			command.append("0");	
		}
		else{
			command.append("1");	
		}
		emit eventTriggered(command);
	}
}

void EventListener::volumeChanged(qint8 mode, qint8 step){
	QString command = "@OSD:FR^VOLUME^";

	if ( step == 0)
		command.append("VOLUME_MUTE^");
	else
		command.append("VOLUME^");

	switch(mode){
	case 0x00:
	case 0x01:
		command.append("Audio");
		break;
	case 0x02:
		command.append("Navi");
		break;
	case 0x09:
	case 0x03:
		command.append("Voice");
		break;
	case 0x04:
		command.append("Bluetooth Phone");
		break;
	case 0x0D:
		command.append("Blue Link Phone");
		break;
	case 0x05:	// Bell
		command.append("Bluetooth Phone");
		break;
	case 0x0E:	// Bell
		command.append("Blue Link Phone");
		break;
	case 0x06:
	case 0x07:
	case 0x08:
	case 0x11:	// DAB TA
		command.append("Info");
		break;
    case 0x0A:
    	command.append("Bluetooth Audio");
        break;
	case 0x0F:
		command.append("Emergency");
	default:
		break;
	}

	command.append("^");
	command.append(QString::number(step));

        lprintf("Command(volumeChanged) : %s", qPrintable(command));

	emit eventTriggered(command);
}
