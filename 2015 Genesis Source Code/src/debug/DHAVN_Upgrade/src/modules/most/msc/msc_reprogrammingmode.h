#ifndef	MSC_REPROGRAMMING_MODE_H_
#define	MSC_REPROGRAMMING_MODE_H_

#include "msc_abstract.h"

#include <QTimer>

class MscReprogrammingMode : public MscAbstract{
Q_OBJECT
public:
	MscReprogrammingMode(QObject * obj, QDBusAbstractInterface * p, SWDLID::SWDLID swdlid);
	~MscReprogrammingMode();
	void start();

private:
	void startReprogramming();
	QTimer * m_bootloaderTimer;
	int m_tryCount;

private slots:
	void ReprogrammingMode(const QDBusVariant &);
	void waitEnterBootloader();
	void notReceiveAck();
	void ReadHWIdentifier(uint, const QString &);
};

#endif
