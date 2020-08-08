#include "powermanagement.h"
#include "msg_processor.h"
#include "protocol_common.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#define TRIGGER_PATH	"/tmp/trigger-standby"

static void connect_to_standby_trigger(){
	struct sockaddr_un	servaddr;
	int fd;
	int ret;
	char buf[1024];

	fd = socket(AF_LOCAL, SOCK_STREAM, 0);
	
	if ( fd < 0 ){
        //gf_err(SYS_ERR, "socket open fail");
        exit(1);
    }
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sun_family = AF_LOCAL;

	if(strlen(TRIGGER_PATH) > sizeof(servaddr.sun_path)){
		close(fd);
		return;
	}

	strncpy(servaddr.sun_path, "/tmp/trigger-standby", sizeof(servaddr.sun_path));
    ret = connect(fd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	if(ret < 0){
		snprintf(buf, 1024, "echo %s > /home/meego/standby_trigger_error_log", strerror(errno));
		system(buf);
        //gf_err(SYS_ERR, "socket connection fail %s", strerror(errno));
		return;
	}

	close(fd);
}

PowerManagement::PowerManagement(QObject * obj)
    : ObjectAbstractInterface(obj){
    adaptor = new PowerManagementAdaptor(this);
	mmusUtil = MMusUtil::instance();
}

void PowerManagement::LgeDBus_SetHardwarePowerKeyControl(uchar in0){
    QDBusMessage msg;
    msg << in0;
    CMsgProcessor::instance()->processDBusCallNoResponse(CMD_HW_POWER_KEY_CONTROL, msg);
}

void PowerManagement::LgeDBus_SetSleepConfirm(){
	::connect_to_standby_trigger();
    CMsgProcessor::instance()->processDBusCallNoResponse(CMD_ETC_SLEEP_CONFIRM, QDBusMessage());
}

void PowerManagement::LgeDBus_HWCameraPowerControl(uchar in0){
    QDBusMessage msg;
    msg << in0;
    CMsgProcessor::instance()->processDBusCallNoResponse(CMD_HW_CAMERA_POWER_CONTROL, msg);
}

void PowerManagement::LgeDBus_BTPowerControl(uchar in0){
    QDBusMessage msg;
    msg << in0;
    CMsgProcessor::instance()->processDBusCallNoResponse(CMD_HW_BT_POWER_CONTROL, msg);
}

void PowerManagement::LgeDBus_MPAutoTestModeCtrl(const QByteArray &in0){
	QDBusMessage msg;
    for (int i = 0; i < in0.count(); ++i)
        msg << in0.at(i);	
	CMsgProcessor::instance()->processDBusCallNoResponse(CMD_MP_AUTOTEST_MODE_CTRL, msg);
}

void PowerManagement::LgeDBus_MPAutoTestModeEvent(const QByteArray &in0){
	QDBusMessage msg;
    for (int i = 0; i < in0.count(); ++i)
        msg << in0.at(i);	
	CMsgProcessor::instance()->processDBusCallNoResponse(CMD_MP_AUTOTEST_MODE_EVENT, msg);
}

void PowerManagement::LgeDBus_AutoTestWIPIDControl(const QByteArray &in0){
	QDBusMessage msg;
    for (int i = 0; i < in0.count(); ++i)
        msg << in0.at(i);	
	CMsgProcessor::instance()->processDBusCallNoResponse(CMD_AUTO_TEST_WIPID_CONTROL, msg);
}

void PowerManagement::LgeDBus_MPAutoModeTestMsg(const QByteArray &in0){
	QDBusMessage msg;
    for (int i = 0; i < in0.count(); ++i)
        msg << in0.at(i);	
	CMsgProcessor::instance()->processDBusCallNoResponse(CMD_MP_AUTO_MODE_TEST_MSG, msg);
}

QByteArray PowerManagement::LgeDBus_MPAutoTestModeRequest(const QByteArray &in0){
	setDelayedReply(true);
	QDBusMessage msg;
    for (int i = 0; i < in0.count(); ++i)
        msg << in0.at(i);	
	CMsgProcessor::instance()->processDBusCallWithResponse(CMD_MP_MODE_INFO_REQUEST, connection(), message().createReply(), msg);
	return QByteArray();
}

QByteArray PowerManagement::LgeDBus_SetMPModeInfo(const QByteArray &in0){
	setDelayedReply(true);
	QDBusMessage msg;
	for (int i = 0; i < in0.count(); ++i)
        msg << in0.at(i);	
	CMsgProcessor::instance()->processDBusCallWithResponse(CMD_ASK_SET_MP_MODE, connection(), message().createReply(), msg);
	return QByteArray();
}

QByteArray PowerManagement::LgeDBus_AskMPModeInfo(const QByteArray &in0){
	setDelayedReply(true);
	QDBusMessage msg;
	for (int i = 0; i < in0.count(); ++i)
        msg << in0.at(i);	
	CMsgProcessor::instance()->processDBusCallWithResponse(CMD_ASK_MP_MODE, connection(), message().createReply(), msg);
	return QByteArray();
}

QByteArray PowerManagement::LgeDBus_AskAutoTestWIPID(){
	setDelayedReply(true);
	CMsgProcessor::instance()->processDBusCallWithResponse(CMD_ASK_AUTO_TEST_WIPID, connection(), message().createReply());
	return QByteArray();
}

bool PowerManagement::handleEvent(quint16 cmd, const QByteArray & data){
	switch(cmd){
		case CMD_ETC_SLEEP_REQUEST:
			if ( data.count()){

				// --> after UISH send sleep confirm we need to connect to standby_trigger
				//if ( data.at(0) == 0x01 ){
				//	::connect_to_standby_trigger();
				//}

				emit LgeDBus_EventRequestSleep( (uchar) data.at(0));
			}
			else
				mmusUtil->gf_err(MY_ERR, "PowerManagment:handleEvent CMD_ETC_SLEEP_CONFIRM arguments not received");

        return true;
    case CMD_ETC_POWER_KEY:
        if ( data.count())
            emit LgeDBus_EventPowerKey( (uchar) data.at(0));
        else
            mmusUtil->gf_err(MY_ERR, "PowerManagment:handleEvent CMD_ETC_POWER_KEY arguments not received");
        return true;
	case CMD_MP_AUTO_MODE_TEST_EVENT:
		char start_cmd[] = "AT+DH_SET_ENABLE_REQ"; 
        if ( data.count()){
			if (strncmp(data.data(), start_cmd, strlen(start_cmd)) == 0)
				emit LgeDBus_MPAutoModeTestStart();
			else
				emit LgeDBus_MPAutoModeTestEvent(data);
		}
        else
            mmusUtil->gf_err(MY_ERR, "PowerManagment:handleEvent CMD_MP_AUTO_MODE_TEST_EVENT arguments not received");
        return true;
    }
    return false;
}
