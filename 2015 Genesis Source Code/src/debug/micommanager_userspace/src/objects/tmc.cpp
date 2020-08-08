#include "tmc.h"
#include "msg_processor.h"
#include "protocol_common.h"

Tmc::Tmc(QObject * obj)
 : ObjectAbstractInterface(obj){
	adaptor = new TMCAdaptor(this);
	mmusUtil = MMusUtil::instance();
}

void Tmc::LgeDBus_SetManualFreqChange(uchar in0, uchar in1){
	QDBusMessage msg;
	msg << in0 << in1;
	CMsgProcessor::instance()->processDBusCallNoResponse(CMD_TMC_MANUAL_FREQ_CHANGE_CONTROL, msg);
}

void Tmc::LgeDBus_SetCountryCodeAndDecodeStatus(const QByteArray &in0){
	QDBusMessage msg;
	for (int i = 0; i < in0.count(); ++i)
        msg << in0.at(i);
	CMsgProcessor::instance()->processDBusCallNoResponse(CMD_TMC_COUNTRY_CODE_DECODE_STATUS, msg);
}

QByteArray Tmc::LgeDBus_AskCurrentFreq(){
	setDelayedReply(true);
	CMsgProcessor::instance()->processDBusCallWithResponse(CMD_ASK_TMC_CURRENT_FREQ, connection(), message().createReply());	
	return QByteArray();
}

QByteArray Tmc::LgeDBus_AskStationList(){
	setDelayedReply(true);
	CMsgProcessor::instance()->processDBusCallWithResponse(CMD_ASK_TMC_STATION_LIST, connection(), message().createReply());	
	return QByteArray();
}

QByteArray Tmc::LgeDBus_AskStationSensitivity(){
	setDelayedReply(true);
	CMsgProcessor::instance()->processDBusCallWithResponse(CMD_ASK_TMC_STATION_SENSITIVITY, connection(), message().createReply());	
	return QByteArray();
}

bool Tmc::handleEvent(quint16 cmd, const QByteArray & data){
	switch(cmd){
	case CMD_TMC_INFO_UPDATE:
		if ( data.count())
			emit LgeDBus_EventInfoUpdate(data);
		else
			mmusUtil->gf_err(MY_ERR, 
					"Tmc:handleEvent CMD_TMC_INFO_UPDATE arguments not received");
		return true;
	case CMD_RDS_FREQ_UPDATE:
		if ( data.count()){
			//mmusUtil->gf_print_debug("MW to DBus(Event)", data);
		}
		else
			mmusUtil->gf_err(MY_ERR, 
					"Tmc:handleEvent CMD_RDS_FREQ_UPDATE arguments not received");
		return true;
	}
	return false;
}
