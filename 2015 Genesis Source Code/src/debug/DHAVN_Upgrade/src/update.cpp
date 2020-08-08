#include "update.h"
#include "usbstorage.h"
#include "controller.h"
#include "common.h"
#include "updatepreparation.h"

#include <QDir>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QEventLoop>

#include <QDebug>

Update::Update(QObject * obj)
: QThread(0){
	m_uiThread = obj;
	m_controller = NULL;
}

Update::~Update(){
}

void Update::run(){
	QEventLoop loop;

         ::sleep(2);
        UpdatePreparation updatePreparation;
        updatePreparation.tryToKillApplications();

	USBStorage			s;
	Controller			controller;
	
	m_controller = &controller;

	QObject::connect(&s, SIGNAL(usbDetached()), m_controller, SLOT(usbDetached()));
	QObject::connect(this, SIGNAL(cannotProceedUpdate(const QString &)), m_uiThread, SLOT(failToUpdate(const QString &)));
	QObject::connect(m_controller, SIGNAL(updateFinished(bool, const QString &)), m_uiThread, SLOT(updateFinished(bool, const QString &)));

	QObject::connect(m_controller, SIGNAL(confirmUSBDetached()), m_uiThread, SLOT(usbDetached()));
	QObject::connect(m_controller, SIGNAL(availableUpdates(int,unsigned int)), m_uiThread, SLOT(availableUpdate(int, unsigned int)));
	QObject::connect(m_controller, SIGNAL(updateProgressChanged(const QString &, const QString &, unsigned int, unsigned int)), m_uiThread, SLOT(progressChanged(const QString &, const QString &, unsigned int, unsigned int)));
	QObject::connect(m_controller, SIGNAL(updateProgressChangedDirect(const QString &, const QString &, unsigned int, unsigned int)), m_uiThread, SLOT(progressChangedDirect(const QString &, const QString &, unsigned int, unsigned int)));
	QObject::connect(m_controller, SIGNAL(cannotProceedUpdate(const QString &)), SIGNAL(cannotProceedUpdate(const QString &)));

	QObject::connect(m_controller, SIGNAL(updateStopped()), m_uiThread, SLOT(goToFailPage()));
	QObject::connect(this, SIGNAL(setMode(int)), m_uiThread, SLOT(setUser(int))); 

	QObject::connect(m_uiThread, SIGNAL(startReboot()), m_controller, SLOT(reboot()));
	QObject::connect(m_uiThread, SIGNAL(stopUpdate()), m_controller, SLOT(stopUpdate()));

        QObject::connect(m_uiThread, SIGNAL(startRebootFailCase()), m_controller, SLOT(failCaserebot()));

	if ( checkUpdate(s.getUSBMountPoint())){
		emit setMode( (int)m_controller->updateMode());
		m_controller->checkAvailableUpdate();
	}

    


	loop.exec();
}

bool Update::checkUpdate(const QString & mountPoint){
	ULOG->log("USB MountPoint : %s", qPrintable(mountPoint));
	QString msg;

	if ( !mountPoint.isEmpty() ){
		Controller::ReturnCode result = m_controller->checkUpdateEnvironment(mountPoint);

		switch(result){
		case Controller::e_ValidateOk:
			return true;
		case Controller::e_FileCorrupted:
			msg = tr("STR_UPGRADE_FILE_CORRUPTED");
			break;
		case Controller::e_NoEssentialFiles:
			msg = tr("STR_UPGRADE_NO_ESSENTIAL");
			break;
		case Controller::e_UpdateKeyCorrupted:
			msg = tr("STR_UPGRADE_FILE_CORRUPTED");
			break;
		case Controller::e_VersionRequireUnmatch:
			msg = tr("STR_UPGRADE_VERSION_REQUIRE_UNMATCH");
			break;
		case Controller::e_UpdateEnvFailed:
			msg = tr("STR_UPGRADE_PREPARE_FAIL") + " (CODE:" + QString::number(m_controller->errorCode()) + ")";
			break;
		default:
			msg = tr("STR_UPGRADE_UNKNOWN_ERROR");
			break;
		}
	}
	else{
		msg = tr("STR_UPGRADE_NO_USB_SD");
	}

	ULOG->log("Update CheckUpdate Error : %s", qPrintable(msg));

	emit cannotProceedUpdate(msg);

	return false;
}
