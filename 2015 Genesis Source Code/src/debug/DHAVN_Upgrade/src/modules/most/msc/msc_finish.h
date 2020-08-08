#ifndef	MSC_FINISH_H_
#define	MSC_FINISH_H_

#include "msc_abstract.h"
#include <QTimer>

class MscFinish : public MscAbstract {
Q_OBJECT
public:
	MscFinish(QObject * obj, QDBusAbstractInterface * p, SWDLID::SWDLID swdlid);
	~MscFinish();
	void start();

private:
	QTimer * m_waitTimer;
	int		m_waitCount;


	void deleteIBoxFiles();
	void deleteAmplifierFiles();

private slots:
	void UpdateProcessing(uint, uint);
	void checkFinishIBoxUpdate();

signals:
	void updateFinished();

	void iBoxWaitCount(int);
};

#endif
