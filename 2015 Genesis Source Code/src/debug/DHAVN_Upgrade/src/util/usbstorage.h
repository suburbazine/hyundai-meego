#ifndef	USB_STORAGE_H_
#define	USB_STORAGE_H_

#include <QObject>
#include <QtDBus>

class USBStorage : public QObject{
Q_OBJECT
public:
	USBStorage(QObject * obj = 0);
	~USBStorage();

	QString getUSBMountPoint();

private:
	QDBusInterface * iUdisk;
	QString			 m_path;
	int isExistUpgradeFile(const QStringList &  mountPoints);

private slots:
	void usbDetached(QDBusObjectPath);

signals:
	void usbDetached();
};

#endif
