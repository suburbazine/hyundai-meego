#ifndef	CONTROLLER_H_
#define	CONTROLLER_H_

#include <QObject>
#include <QTimer>

class UpdateElement;
class CheckPartition;
class UpdateLauncher;

class Controller : public QObject{
Q_OBJECT
public:
	enum ReturnCode{
		e_ValidateOk = 0,
		e_AvailableUpdate,
		e_NoAvailableUpdate,
		e_FileCorrupted,
		e_NoEssentialFiles,
		e_UpdateKeyCorrupted,
		e_UpdateEnvFailed,
		e_VersionRequireUnmatch
	};

	enum UpdateState {
		u_CheckEnvironment,
		u_CheckAvailableUpdate,
		u_PrepareFiles,
		u_StartUpdates
	};

	Controller(QObject * obj = 0);
	~Controller();
	ReturnCode checkUpdateEnvironment(const QString & mountPath);

	int errorCode() { return m_errorCode; }
	int updateMode();

	int countOfPartitionUpdate();

	void checkPartitionsChecksum();
	void checkAvailableUpdate();

private:
	int m_errorCode;

	bool m_stopUpdate;

	UpdateState m_updateState;

	QList<UpdateElement *> * m_updateList;

	CheckPartition * m_checkPartition;
	UpdateLauncher * m_updateLauncher;

	void startUpdate();
	bool isMicomUpdateExist();
        bool isMicomAndFLCDUpdateExist();
	bool canChangePartition();

private slots:
	void usbDetached();
	void reboot();
        void failCaserebot(); 
	void stopUpdate();

	void checkPartitionFinished(int);

	void cleanupUpdate();

signals:
	void confirmUSBDetached();
	void availableUpdates(int, unsigned int);

	void updateFinished(bool, const QString &);
	void cannotProceedUpdate(const QString &);

	void updateProgressChanged(const QString & name,
								const QString & description,
								unsigned int progress,
								unsigned int weight_progress);
	void updateProgressChangedDirect(const QString & name,
								const QString & description,
								unsigned int progress,
								unsigned int weight_progress);
	void updateStopped();
};



#endif
