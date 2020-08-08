#include "system.h"
#include "protocol_common.h"

System::System(QObject * obj)
    : ObjectAbstractInterface(obj){
    adaptor = new SystemAdaptor(this);
	mmusUtil = MMusUtil::instance();
}

QByteArray System::LgeDBus_AskRTCInfo(){
    setDelayedReply(true);
    CMsgProcessor::instance()->processDBusCallWithResponse(CMD_ASK_RTC_INFO,
                                               connection(), message().createReply());
    return QByteArray();
}

void System::LgeDBus_EEPROMWrite(const QByteArray &in0)
{
    QDBusMessage msg;
    for (int i = 0; i < in0.count(); ++i)
        msg << (uchar) in0.at(i);
    CMsgProcessor::instance()->processDBusCallNoResponse(CMD_EEPROM_WRITE, msg);
}

QByteArray System::LgeDBus_EEPROMRead(const QByteArray &in0)
{
    setDelayedReply(true);
	QDBusMessage msg;
    for (int i = 0; i < in0.count(); ++i)
        msg << (uchar) in0.at(i);
    CMsgProcessor::instance()->processDBusCallWithResponse(CMD_EEPROM_READ,
                                               connection(), message().createReply(), msg);
    return QByteArray();
}

void System::LgeDBus_SetFanControl(uchar in0){
    QDBusMessage msg;
    msg << in0;
    CMsgProcessor::instance()->processDBusCallNoResponse(CMD_HW_FAN_CONTROL, msg);
}

void System::LgeDBus_SetHwRTCSet(const QByteArray &in0){
    QDBusMessage msg;
    for (int i = 0; i < in0.count(); ++i)
        msg << (uchar) in0.at(i);
    CMsgProcessor::instance()->processDBusCallNoResponse(CMD_HW_RTC_SET, msg);
}

void System::LgeDBus_SetPhoneMicControl(uchar in0){
    QDBusMessage msg;
    msg << in0;
    CMsgProcessor::instance()->processDBusCallNoResponse(CMD_HW_PHONE_MIC_CONTROL, msg);
}

void System::LgeDBus_AppBootComplete(uchar in0)
{
    QDBusMessage msg;
    msg << in0;
    CMsgProcessor::instance()->processDBusCallNoResponse(CMD_APP_BOOT_COMPLETE, msg);
}

void System::LgeDBus_SetDiscIndicationLed(uchar in0){
    QDBusMessage msg;
    msg << in0;
    CMsgProcessor::instance()->processDBusCallNoResponse(CMD_HW_DISC_INDICATION_LED_CONTROL, msg);
}

void System::LgeDBus_AVMInitialRequest(){
    QDBusMessage msg;
    CMsgProcessor::instance()->processDBusCallNoResponse(CMD_AVM_INITIAL_REQUEST, msg);

}

void System::LgeDBus_CarInfoInitialRequest(){
    QDBusMessage msg;
    CMsgProcessor::instance()->processDBusCallNoResponse(CMD_CAR_INFO_INITIAL_REQUEST, msg);

}

void System::LgeDBus_CarSetInitialRequest(){
    QDBusMessage msg;
    CMsgProcessor::instance()->processDBusCallNoResponse(CMD_CAR_SET_INITIAL_REQUEST, msg);

}

void System::LgeDBus_DATCInitialRequest(){
    QDBusMessage msg;
    CMsgProcessor::instance()->processDBusCallNoResponse(CMD_DATC_INITIAL_REQUEST, msg);

}

void System::LgeDBus_PGSInitialRequest(){
    QDBusMessage msg;
    CMsgProcessor::instance()->processDBusCallNoResponse(CMD_PGS_INITIAL_REQUEST, msg);
}

void System::LgeDBus_ResetHWPeripheral(const QByteArray &in0){
    QDBusMessage msg;
	for(int i = 0 ; i < in0.count() ; ++i)
		msg << (uchar) in0.at(i);

    CMsgProcessor::instance()->processDBusCallNoResponse(CMD_HW_PERIPHERAL_RESET, msg);
}

void System::LgeDBus_MainImageUpgrade(uchar in0)
{
    QDBusMessage msg;
    msg << in0;
    CMsgProcessor::instance()->processDBusCallNoResponse(CMD_MAIN_IMAGE_UPGRADE, msg);
}

void System::LgeDBus_ETCFactorySetting(uchar in0)
{
    QDBusMessage msg;
    msg << in0;
    CMsgProcessor::instance()->processDBusCallNoResponse(CMD_ETC_FACTORY_SETTING, msg);
}

void System::LgeDBus_WDTCountClear(){
    QDBusMessage msg;
    CMsgProcessor::instance()->processDBusCallNoResponse(CMD_WDT_COUNT_CLEAR, msg);

}

QByteArray System::LgeDBus_WDTCountInfo(){
    setDelayedReply(true);
    CMsgProcessor::instance()->processDBusCallWithResponse(CMD_WDT_COUNT_INFO,
                                               connection(), message().createReply());
    return QByteArray();
}

bool System::handleEvent(quint16 cmd, const QByteArray & data){
	switch(cmd){
	case CMD_ETC_FRONT_SPECIAL_KEY:
		if ( data.count())
			emit LgeDBus_EventFrontSpecialKey( (uchar)data.at(0));
		else
            mmusUtil->gf_err(MY_ERR, 
					"System:handleEvent CMD_ETC_FRONT_SPECIAL_KEY arguments not received");
		return true;
	case CMD_FACTORY_RESET_COMPLETE:
		if ( data.count())
			emit LgeDBus_FactoryResetComplete( (uchar)data.at(0));
		else
            mmusUtil->gf_err(MY_ERR, 
					"System:handleEvent CMD_ETC_FRONT_SPECIAL_KEY arguments not received");
		return true;
	}
    return false;
}
