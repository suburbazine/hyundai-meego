#include "updatepreparation.h"
#include "meegoeuid.h"
#include <QDBusConnection>
#include <QDBusMessage>
#include <sys/mount.h>
#include <sys/types.h>

#include "common.h"


UpdatePreparation::UpdatePreparation(QObject * obj)
: QObject(obj)
{

}

void UpdatePreparation::tryToKillApplications(){
	::system("pgrep_kill App");

	::sleep(2);
	if(::mount("/dev/mmcblk0p1", "/navi", "vfat", MS_RDONLY, NULL) == 0){
		ULOG->log("SD card remount to /navi for update");
		::sleep(1);
	}
}

void UpdatePreparation::sendUpdateInfoToMicom(){
	MeegoEUID meegoEUID;
	QDBusConnection conn = QDBusConnection::sessionBus();
	QDBusMessage msg;

	if ( !conn.isConnected()){
		ULOG_EXCEPTION;
		ULOG->log("Failed to connect session Bus");
	}
	else{
		msg = QDBusMessage::createMethodCall("com.lge.car.micom",
											"/System", "com.lge.car.micom.system",
											"LgeDBus_MainImageUpgrade");
		uchar value = 0x01;
		msg << qVariantFromValue(value);
		if ( !conn.send(msg)){
			ULOG->log("Failed to send MainImageUpgrade");
		}
	}
}
