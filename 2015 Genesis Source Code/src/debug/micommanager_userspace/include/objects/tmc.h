#ifndef	TMC_H
#define	TMC_H

#include "object_abstractinterface.h"
#include "tmc_adaptor.h"
#include "util.h"
#include <QDBusContext>

class Tmc : public ObjectAbstractInterface, public QDBusContext {
	Q_OBJECT
	TMCAdaptor * adaptor;
public:
	Tmc(QObject * obj = 0);
	bool handleEvent(quint16, const QByteArray & data = QByteArray());

public Q_SLOTS:
	void LgeDBus_SetManualFreqChange(uchar in0, uchar in1);
    void LgeDBus_SetCountryCodeAndDecodeStatus(const QByteArray &in0);
	QByteArray	LgeDBus_AskCurrentFreq();
	QByteArray	LgeDBus_AskStationList();
	QByteArray	LgeDBus_AskStationSensitivity();

Q_SIGNALS:
	void LgeDBus_EventInfoUpdate(const QByteArray &);
	void LgeDBus_EventRdsFreqUpdate(const QByteArray &);
private:
	MMusUtil * mmusUtil;
};

#endif
