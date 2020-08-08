#ifndef	FIRMWARE_UPDATER_H_
#define	FIRMWARE_UPDATER_H_

#include "abstractupdater.h"
#include "firmware.h"
#include <QTimer>

class MessageEvent;

class FirmwareUpdater : public AbstractUpdater {
Q_OBJECT
public:
	FirmwareUpdater(QObject * obj = 0);
	~FirmwareUpdater();

	void setUpdateList(const QList<UpdateElement *> & list);
	void start();
	void stop();
	bool finish();

private:
	QList<UpdateElement *> m_targets;
	QList<UpdateElement *> m_waitTargets;
	QList<UpdateElement *> m_runningTargets;

	QTimer * m_launcherTimer;

	void popWaitQueueAndRun(int i);
	bool isCanDevice(const QString & name);
	bool checkRunningFirmware(Firmware::DependencyCode);
	bool checkReadyFirmware(Firmware::DependencyCode);

	void startLauncher(UpdateElement * element, const QString & launcher);
	void checkTimeoutRunningList();
	bool isFinished();
        bool bios_check;

	MessageEvent * m_messageEvent;

	UpdateElement * getElementFromRunningList(int id);
	UpdateElement * getElementFromAll(int id);

private slots:
	void startLauncher();
	void checkFirmwareUpdater(int, int, int, const QString &);
};

#endif
