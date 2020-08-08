#ifndef DTC_H
#define DTC_H

#include "dtc_adaptor.h"
#include "object_abstractinterface.h"
#include "util.h"
#include <QDBusContext>
#include <QUdpSocket>

class DTC : public ObjectAbstractInterface, public QDBusContext {
    Q_OBJECT
    DTCAdaptor * adaptor;
	QUdpSocket * udpSocket_recv;
public :
    DTC(QObject * obj = 0);
    bool handleEvent(quint16, const QByteArray & data = QByteArray());

public Q_SLOTS : //METHODS
    void LgeDBus_CANTPSendMsg(const QByteArray &in0);
    QByteArray LgeDBus_AskMOST_RBDRequest();
    void LgeDBus_AutoTestKeyControl(const QByteArray &in0);
	void LgeDBus_MOSTResetRequest();
	void LgeDBus_CANTPMicomRecv();
    void LgeDBus_DTCClearRequest(uchar in0);
    void LgeDBus_DTCReadRequest(const QByteArray &in0);
    void LgeDBus_DTCSendMsg(const QByteArray &in0);
    void LgeDBus_AuxMicCompleteMsg(uchar in0);
    void LgeDBus_NaviSCCDateSend(const QByteArray &in0);
	void LgeDBus_TouchCleanup();
    void LgeDBus_KeyTouchRandomTest(const QByteArray &in0);
Q_SIGNALS: // SIGNALS
    void LgeDBus_EventDTCReadResponse(const QByteArray &in0);
    void LgeDBus_EventDTCClearResponse(const QByteArray &in0);
    void LgeDBus_EventCANTPMicomSend(const QByteArray &in0);
    void LgeDBus_EventEDT(const QByteArray &in0);
    void LgeDBus_AuxMicStatusEvent(uchar in0);
    void LgeDBus_NaviSCCMsgTriggerEvent();

private:
	MMusUtil * mmusUtil;

};

#endif
