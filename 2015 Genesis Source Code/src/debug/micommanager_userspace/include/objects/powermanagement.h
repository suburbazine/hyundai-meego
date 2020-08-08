#ifndef _POWERMANAGEMENT_H_
#define _POWERMANAGEMENT_H_

#include "object_abstractinterface.h"
#include <QDBusContext>
#include <QDBusConnection>
#include "powermanagement_adaptor.h"
#include "util.h"

class PowerManagement : public ObjectAbstractInterface, public QDBusContext {
    Q_OBJECT
    PowerManagementAdaptor * adaptor;
public:
    PowerManagement(QObject * obj = 0);
    bool handleEvent(quint16, const QByteArray & data = QByteArray());

public Q_SLOTS: // METHODS
    void LgeDBus_SetHardwarePowerKeyControl(uchar in0);
    void LgeDBus_SetSleepConfirm();
    void LgeDBus_BTPowerControl(uchar in0);
    void LgeDBus_HWCameraPowerControl(uchar in0);
    void LgeDBus_MPAutoModeTestMsg(const QByteArray &in0);
    void LgeDBus_MPAutoTestModeCtrl(const QByteArray &in0);
    void LgeDBus_MPAutoTestModeEvent(const QByteArray &in0);
    void LgeDBus_AutoTestWIPIDControl(const QByteArray &in0);
    QByteArray LgeDBus_MPAutoTestModeRequest(const QByteArray &in0);
	QByteArray LgeDBus_SetMPModeInfo(const QByteArray &in0);
	QByteArray LgeDBus_AskMPModeInfo(const QByteArray &in0);
    QByteArray LgeDBus_AskAutoTestWIPID();
Q_SIGNALS: // SIGNALS
    void LgeDBus_EventPowerKey(uchar in0);
    void LgeDBus_EventRequestSleep(uchar in0);
    void LgeDBus_MPAutoModeTestEvent(const QByteArray &in0);
    void LgeDBus_MPAutoModeTestStart();
private:
	MMusUtil * mmusUtil;
};


#endif
