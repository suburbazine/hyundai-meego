#ifndef SYSTEM_H
#define SYSTEM_H

#include "object_abstractinterface.h"
#include "system_adaptor.h"
#include "util.h"
#include <QDBusContext>

class System : public ObjectAbstractInterface, public QDBusContext{
    Q_OBJECT
    SystemAdaptor * adaptor;
public:
    System(QObject * obj = 0);
    bool handleEvent(quint16, const QByteArray & data = QByteArray());

public Q_SLOTS: // METHODS
    void LgeDBus_AVMInitialRequest();
    QByteArray LgeDBus_AskRTCInfo();
    QByteArray LgeDBus_WDTCountInfo();
	QByteArray LgeDBus_EEPROMRead(const QByteArray &in0);
    void LgeDBus_WDTCountClear();
    void LgeDBus_AppBootComplete(uchar in0);
    void LgeDBus_EEPROMWrite(const QByteArray &in0);
    void LgeDBus_CarInfoInitialRequest();
    void LgeDBus_CarSetInitialRequest();
    void LgeDBus_DATCInitialRequest();
    void LgeDBus_PGSInitialRequest();
    void LgeDBus_SetFanControl(uchar in0);
    void LgeDBus_SetHwRTCSet(const QByteArray &in0);
    void LgeDBus_SetPhoneMicControl(uchar in0);
    void LgeDBus_SetDiscIndicationLed(uchar in0);
	void LgeDBus_ResetHWPeripheral(const QByteArray &in0);
    void LgeDBus_MainImageUpgrade(uchar in0);
    void LgeDBus_ETCFactorySetting(uchar in0);
Q_SIGNALS: // SIGNALS
    void LgeDBus_EventFrontSpecialKey(uchar in0);
    void LgeDBus_FactoryResetComplete(uchar in0);
private:
	MMusUtil * mmusUtil;
};

#endif // SYSTEM_H
