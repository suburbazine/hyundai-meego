#ifndef SYSINFO_H
#define SYSINFO_H


#include "object_abstractinterface.h"
#include "sysinfo_adaptor.h"
#include "util.h"
#include <QDBusContext>

class SysInfo : public ObjectAbstractInterface, public QDBusContext{
    Q_OBJECT
    SysInfoAdaptor * adaptor;
public:
    SysInfo(QObject * obj = 0);
    bool handleEvent(quint16, const QByteArray & data = QByteArray());

public Q_SLOTS: // METHODS
    QByteArray LgeDBus_AskLanguageInfoValue();
    QByteArray LgeDBus_AskSysBootType();
    QByteArray LgeDBus_AskSysLastStatus();
    QByteArray LgeDBus_AskSysOption();
    QByteArray LgeDBus_AskSettingValueMsg();
    QByteArray LgeDBus_AskHDRadioVersion();
    QByteArray LgeDBus_AskMOST_INICVersion();
    QByteArray LgeDBus_AskSysVersion();
    QByteArray LgeDBus_AskBTMACAddr();
    QByteArray LgeDBus_AskAMPVersion();
    void LgeDBus_CPUAliveMsg();
    void LgeDBus_EtcGUIMode(uchar in0);
    void LgeDBus_SetLanguageInfo(uchar in0);
    void LgeDBus_SetSysOption(const QByteArray &in0);
    void LgeDBus_EmergencyConditionMsg(const QByteArray &in0);
    void LgeDBus_SettingValueStatusMsg(const QByteArray &in0);
    void LgeDBus_SwVersionMsg(const QByteArray &in0);
    void LgeDBus_BTMACAddrStore(const QByteArray &in0);

Q_SIGNALS: // SIGNALS
    void LgeDBus_EventCarIFStatus(uchar in0, uchar in1);
    void LgeDBus_EventDayNightStatus(uchar in0);
    void LgeDBus_PopUpEvent(const QByteArray &in0);
    void LgeDBus_AudioMixCHCtrl(const QByteArray &in0);
    void LgeDBus_AudioMainCHCtrl(const QByteArray &in0);
    void LgeDBus_AudioMuteCtrl(const QByteArray &in0);
    void LgeDBus_AudioLastMemory(const QByteArray &in0);
private:
	MMusUtil * mmusUtil;
};

#endif // SYSINFO_H
