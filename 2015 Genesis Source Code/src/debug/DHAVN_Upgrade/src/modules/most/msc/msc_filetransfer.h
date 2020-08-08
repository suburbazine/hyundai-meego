#ifndef	MSC_FILETRANSFER_H_
#define	MSC_FILETRANSFER_H_

#include "msc_abstract.h"
#include <QFile>

class MscFileTransfer : public MscAbstract{
Q_OBJECT
public:
	MscFileTransfer(QObject * obj, QDBusAbstractInterface * p, SWDLID::SWDLID swdlid);
	~MscFileTransfer();

	void start();

	bool retry_ibox;
private:
	MOSTFileCheckSum * m_files;

	void startPrepareFileTransfer();
	void startPrepareFileTransferPassive();
	void sendCheckActivateModule();
	MOSTFileCheckSum * nextFile();
	void requestTransferExit();

	void startTransferData();
	void transferBlock();

	quint64	m_allBlockCount;
	quint64 m_currentBlockCount;

	QFile	m_currentFile;
	STransferDataCMSAMS m_transferSource;
	QTimer * timer; 

	int disconnect_count;
private slots:
	void PrepareFileTransfer(const QDBusVariant &);
	void RequestTransferExit(const QDBusVariant &);
	void CheckActivateModule(const QDBusVariant &);
	void TransferData(const QDBusVariant &);
        void TimeoutRequestTransferExit();  
        void iBoxBlockVisibleSlot(const bool state);

signals:
	void transfered(quint64, quint64);
};

#endif
