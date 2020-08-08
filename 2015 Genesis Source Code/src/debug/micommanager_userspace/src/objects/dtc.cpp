#include "dtc.h"
#include "msg_processor.h"
#include "protocol_common.h"

#define HOST_ADDR	"127.0.0.1"
#define HOST_PORT_SEND	45500  //wds
#define HOST_PORT_RECV	45460  //khy

DTC::DTC(QObject * obj) : ObjectAbstractInterface(obj){
	adaptor = new DTCAdaptor(this);
	mmusUtil = MMusUtil::instance();
	udpSocket_recv = new QUdpSocket(this);
	udpSocket_recv->bind(HOST_PORT_RECV, QUdpSocket::ShareAddress);

	connect(udpSocket_recv, SIGNAL(readyRead()), this, SLOT(LgeDBus_CANTPMicomRecv()));
}

void DTC::LgeDBus_DTCClearRequest(uchar in0){
    QDBusMessage msg;
    msg << in0;
 
    CMsgProcessor::instance()->processDBusCallNoResponse(CMD_DTC_CLEAR_REQUEST, msg);
}

void DTC::LgeDBus_AuxMicCompleteMsg(uchar in0){
    QDBusMessage msg;
    msg << in0;
 
    CMsgProcessor::instance()->processDBusCallNoResponse(CMD_AUX_MIC_COMPLETE_MSG, msg);
}

void DTC::LgeDBus_TouchCleanup(){
	QDBusMessage msg;
	CMsgProcessor::instance()->processDBusCallNoResponse(CMD_TOUCH_CLEANUP, msg);
}

void DTC::LgeDBus_DTCReadRequest(const QByteArray &in0){
    QDBusMessage msg;
    for (int i = 0; i < in0.count(); ++i)
        msg << in0.at(i);

    CMsgProcessor::instance()->processDBusCallNoResponse(CMD_DTC_READ_REQUEST, msg);
}

void DTC::LgeDBus_DTCSendMsg(const QByteArray &in0){
    QDBusMessage msg;
    for (int i = 0; i < in0.count(); ++i)
        msg << in0.at(i);
    
    CMsgProcessor::instance()->processDBusCallNoResponse(CMD_DTC_SEND_MSG, msg);
}

void DTC::LgeDBus_CANTPSendMsg(const QByteArray &in0){
	QDBusMessage msg;
    for (int i = 0; i < in0.count(); ++i)
        msg << in0.at(i);

	CMsgProcessor::instance()->processDBusCallNoResponse(CMD_CAN_TP_SEND_MSG, msg);
}

QByteArray DTC::LgeDBus_AskMOST_RBDRequest(){
    setDelayedReply(true);
 
    CMsgProcessor::instance()->processDBusCallWithResponse(CMD_ASK_MOST_RBD_REQUEST, connection(), message().createReply());
    return QByteArray();
}

void DTC::LgeDBus_MOSTResetRequest(){
    QDBusMessage msg;
    CMsgProcessor::instance()->processDBusCallNoResponse(CMD_ASK_MOST_RESET_REQUEST, msg);

}

void DTC::LgeDBus_AutoTestKeyControl(const QByteArray &in0){
	QDBusMessage msg;
    for (int i = 0; i < in0.count(); ++i)
        msg << in0.at(i);

	CMsgProcessor::instance()->processDBusCallNoResponse(CMD_AUTO_TEST_KEY_CONTROL, msg);
}

void DTC::LgeDBus_NaviSCCDateSend(const QByteArray &in0){
	QDBusMessage msg;
    for (int i = 0; i < in0.count(); ++i)
        msg << in0.at(i);

	CMsgProcessor::instance()->processDBusCallNoResponse(CMD_NAVI_SCC_DATA_SEND, msg);
}

void DTC::LgeDBus_KeyTouchRandomTest(const QByteArray &in0){
    QDBusMessage msg;
    for (int i = 0; i < in0.count(); ++i)
        msg << in0.at(i);

    CMsgProcessor::instance()->processDBusCallNoResponse(CMD_KEY_TOUCH_RANDOM_TEST, msg);
}

void DTC::LgeDBus_CANTPMicomRecv(){
    udpSocket_recv->hasPendingDatagrams();
	
	QDBusMessage msg;
	QByteArray recv_data;
	QHostAddress sender;
	quint16 senderPort;

	recv_data.clear();
	recv_data.resize(udpSocket_recv->pendingDatagramSize());
	udpSocket_recv->readDatagram(recv_data.data(), recv_data.size(), &sender, &senderPort);
   
	for (int i = 0; i < recv_data.count(); ++i)
        msg << recv_data.at(i);
	CMsgProcessor::instance()->processDBusCallNoResponse(CMD_CAN_TP_MICOM_RECV, msg);
}

bool DTC::handleEvent(quint16 cmd, const QByteArray & data){
	quint16 parse_cmd;

	switch(cmd){
	case CMD_DTC_READ_RESPONSE:
		if ( data.count()) 
			emit LgeDBus_EventDTCReadResponse(data);
		else
            mmusUtil->gf_err(MY_ERR, 
					"System:handleEvent CMD_DTC_READ_RESPONSE arguments not received");
		return true;
	case CMD_DTC_CLEAR_RESPONSE:
		if ( data.count())
			emit LgeDBus_EventDTCClearResponse(data);
		else
            mmusUtil->gf_err(MY_ERR, 
					"System:handleEvent CMD_DTC_CLEAR_RESPONSE arguments not received");
		return true;
	case CMD_CAN_TP_MICOM_SEND:
		if ( data.count()){
			udpSocket_recv->writeDatagram(data.data(), data.size(), QHostAddress::LocalHost, HOST_PORT_SEND);
		}
		else
            mmusUtil->gf_err(MY_ERR, 
					"System:handleEvent CMD_CAN_TP_MICOM_SEND arguments not received");
		return true;
	case CMD_EDT_EVENT:
		if ( data.count()) 
			emit LgeDBus_EventEDT(data);
		else
            mmusUtil->gf_err(MY_ERR, 
					"System:handleEvent CMD_EDT_EVENT arguments not received");
		return true;
	case CMD_AUX_MIC_STATUS_EVENT:
		if ( data.count()) 
			emit LgeDBus_AuxMicStatusEvent(data[0]);
		else
            mmusUtil->gf_err(MY_ERR, 
					"System:handleEvent CMD_EDT_EVENT arguments not received");
		return true;
	case CMD_NAVI_SCC_MSG_TRIGGER:
		/* do not need to check data length */
		emit LgeDBus_NaviSCCMsgTriggerEvent();
		
		return true;

	}
    return false;
}
