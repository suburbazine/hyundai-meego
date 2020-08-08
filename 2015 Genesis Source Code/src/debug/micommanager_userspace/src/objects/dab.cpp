#include "dab.h"
#include "msg_processor.h"
#include "protocol_common.h"

DAB::DAB(QObject * obj)
	: ObjectAbstractInterface(obj){
    adaptor = new DABAdaptor(this);
	mmusUtil = MMusUtil::instance();
}

void DAB::LgeDBus_DABPICodeRequest(const QByteArray &in0){
    QDBusMessage msg;
    for (int i = 0; i < in0.count(); ++i)
        msg << in0.at(i);
    CMsgProcessor::instance()->processDBusCallNoResponse(CMD_DAB_PI_CODE_REQUEST, msg);
}

void DAB::LgeDBus_DABTAStatus(uchar in0){
    QDBusMessage msg;
    msg << in0;
    CMsgProcessor::instance()->processDBusCallNoResponse(CMD_DAB_TA_STATUS_REQUEST, msg);
}

bool DAB::handleEvent(quint16 cmd, const QByteArray & data){
    switch(cmd){
    case CMD_DAB_PI_SENSITIVITY:
        if ( data.count())
            emit LgeDBus_EventDABPISensitivity(data);
        else
            mmusUtil->gf_err(MY_ERR, 
					"SysInfo:handleEvent CMD_ETC_CAR_IF_STATUS arguments problem");
        return true;
	}

    return false;
}
