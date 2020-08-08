#include "usbstorage.h"
#include <QFile>
#include <sys/mount.h>
#include "common.h"
#define CONFIGURATION_FILE			"/.lge.upgrade.xml"

extern bool usb_status;

USBStorage::USBStorage(QObject * obj)
: QObject(obj){
	iUdisk = new QDBusInterface( "org.freedesktop.UDisks",
                                 "/org/freedesktop/UDisks",
                                 "org.freedesktop.UDisks",
                                              QDBusConnection::systemBus() );
	Q_CHECK_PTR(iUdisk);
	iUdisk->connection().connect( "org.freedesktop.UDisks", 
								"/org/freedesktop/UDisks",
                                "org.freedesktop.UDisks", 
								"DeviceRemoved",
                                this, SLOT( usbDetached( QDBusObjectPath ) ) );
}

USBStorage::~USBStorage(){
	if ( iUdisk )
		delete iUdisk;
}

void USBStorage::usbDetached(QDBusObjectPath obj){

	QDBusInterface deviceInterface( "org.freedesktop.UDisks",
                                     obj.path(),
                                   "org.freedesktop.UDisks.Device",
							       QDBusConnection::systemBus() );
	bool isUnmounted = deviceInterface.property("DeviceIsMounted").toBool();
	if ( !isUnmounted && obj.path() == m_path){
		qDebug() << "USB Detached!!!";
		m_path = "";
		emit usbDetached();
	}
}

QString USBStorage::getUSBMountPoint(){
	QDBusInterface iface( "org.freedesktop.UDisks",
						"/org/freedesktop/UDisks",
						"org.freedesktop.UDisks",
						QDBusConnection::systemBus() );
	QDBusReply<QList<QDBusObjectPath> > reply = iface.call("EnumerateDevices");
	QList<QDBusObjectPath> devices = reply.value();
	for (int i = 0; i < devices.count(); ++i){
		QDBusInterface deviceInterface( "org.freedesktop.UDisks",
								devices.at(i).path(),
								"org.freedesktop.UDisks.Device",
								QDBusConnection::systemBus());	
		bool isMounted = deviceInterface.property("DeviceIsMounted").toBool();
		bool isRemovable = deviceInterface.property("DeviceIsRemovable").toBool();
		bool isUSB = deviceInterface.property("DriveConnectionInterface").toString().contains("usb", Qt::CaseInsensitive);
        bool isMMC = deviceInterface.property("DriveConnectionInterface").toString().contains("sdio", Qt::CaseInsensitive); //jwyang sdcard Upgrade
		QString deviceFile = deviceInterface.property("DeviceFile").toString();

		if ( !isMounted && deviceFile.contains("mmcblk0p1")){
			if(::mount("/dev/mmcblk0p1", "/navi", "vfat", MS_RDONLY, NULL) == 0){
				ULOG->log("SD card remount to /navi for update");
				::sleep(1);
			}
			isMMC = true;
			isMounted = true;
		}
        
		if(isUSB && isMMC)//jwyang sdcard Upgrade
        {
            isMMC=false;
        }
        
		if (isMounted && isUSB){
              usb_status = true;
        }

        if ( isMounted && (isUSB || isMMC)){  //jwyang sdcard Upgrade
			int index = -1;
			QStringList list = deviceInterface.property("DeviceMountPaths").toStringList();

			if ( (index = isExistUpgradeFile(list)) >= 0){
				m_path = devices.at(i).path();
                                ULOG->log("USB Mount Point list.at(index) : %s " , qPrintable(list.at(index)));
                                if(list.at(index).contains("rear_usb"))
                                    continue;

				return list.at(index);
			}
		}
	}
	return QString();
}

// return : stringlist index
int USBStorage::isExistUpgradeFile(const QStringList &  mountPoints){
	for (int i = 0; i < mountPoints.count(); ++i)
		if ( QFile::exists(mountPoints.at(i) + CONFIGURATION_FILE))
			return i;

	return -1;
}
