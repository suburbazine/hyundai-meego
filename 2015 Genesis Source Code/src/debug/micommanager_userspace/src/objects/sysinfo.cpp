#include "sysinfo.h"
#include "msg_processor.h"
#include "protocol_common.h"

SysInfo::SysInfo(QObject *obj)
    : ObjectAbstractInterface(obj){
    adaptor = new SysInfoAdaptor(this);
	mmusUtil = MMusUtil::instance();
}

QByteArray SysInfo::LgeDBus_AskLanguageInfoValue()
{
    setDelayedReply(true);
    CMsgProcessor::instance()->processDBusCallWithResponse(CMD_ASK_LANGUAGE_INFO_VALUE,
                                               connection(), message().createReply());
    return QByteArray();
}

QByteArray SysInfo::LgeDBus_AskSysBootType(){
    setDelayedReply(true);
    CMsgProcessor::instance()->processDBusCallWithResponse(CMD_ASK_SYS_BOOT_TYPE,
                                               connection(), message().createReply());
    return QByteArray();
}

QByteArray SysInfo::LgeDBus_AskSysLastStatus(){
    setDelayedReply(true);
    CMsgProcessor::instance()->processDBusCallWithResponse(CMD_ASK_SYS_LAST_STATUS,
                                               connection(), message().createReply());
    return QByteArray();
}


QByteArray SysInfo::LgeDBus_AskSysOption(){
    setDelayedReply(true);
    CMsgProcessor::instance()->processDBusCallWithResponse(CMD_ASK_SYS_OPTION_SET,
                                               connection(), message().createReply());
    return QByteArray();
}

QByteArray SysInfo::LgeDBus_AskSysVersion(){
    setDelayedReply(true);
    CMsgProcessor::instance()->processDBusCallWithResponse(CMD_ASK_SYS_VERSION,
                                               connection(), message().createReply());
    return QByteArray();
}

QByteArray SysInfo::LgeDBus_AskHDRadioVersion(){
    setDelayedReply(true);
    CMsgProcessor::instance()->processDBusCallWithResponse(CMD_ASK_HDRADIO_VERSION,
                                               connection(), message().createReply());
    return QByteArray();
}

QByteArray SysInfo::LgeDBus_AskMOST_INICVersion(){
    setDelayedReply(true);
    CMsgProcessor::instance()->processDBusCallWithResponse(CMD_ASK_MOST_INIC_VERSION,
                                               connection(), message().createReply());
    return QByteArray();
}

void SysInfo::LgeDBus_SetLanguageInfo(uchar in0)
{
    QDBusMessage msg;
    msg << in0;
    CMsgProcessor::instance()->processDBusCallNoResponse(CMD_SET_LANGUAGE_INFO, msg);
}

void SysInfo::LgeDBus_EtcGUIMode(uchar in0)
{
    QDBusMessage msg;
    msg << in0;
    CMsgProcessor::instance()->processDBusCallNoResponse(CMD_ETC_GUI_MODE, msg);
}

void SysInfo::LgeDBus_SetSysOption(const QByteArray &in0){
    QDBusMessage msg;
    for (int i = 0; i < in0.count(); ++i)
        msg << in0.at(i);
    CMsgProcessor::instance()->processDBusCallNoResponse(CMD_ETC_OPTION_SET, msg);
}

void SysInfo::LgeDBus_CPUAliveMsg()
{
    CMsgProcessor::instance()->processDBusCallNoResponse(CMD_CPU_ALIVE_MSG, QDBusMessage());
}

void SysInfo::LgeDBus_EmergencyConditionMsg(const QByteArray &in0){
    QDBusMessage msg;
    for (int i = 0; i < in0.count(); ++i)
        msg << in0.at(i);
    CMsgProcessor::instance()->processDBusCallNoResponse(CMD_EMERGENCY_CONDITION_MSG, msg);
}

void SysInfo::LgeDBus_SettingValueStatusMsg(const QByteArray &in0){
    QDBusMessage msg;
    for (int i = 0; i < in0.count(); ++i)
        msg << in0.at(i);
    CMsgProcessor::instance()->processDBusCallNoResponse(CMD_SETTING_VALUE_STATUS_MSG, msg);
}

QByteArray SysInfo::LgeDBus_AskSettingValueMsg(){
    setDelayedReply(true);
    CMsgProcessor::instance()->processDBusCallWithResponse(CMD_ASK_SETTING_VALUE_MSG,
                                               connection(), message().createReply());
    return QByteArray();
}

void SysInfo::LgeDBus_SwVersionMsg(const QByteArray &in0){
    QDBusMessage msg;
    for (int i = 0; i < in0.count(); ++i)
        msg << in0.at(i);
    CMsgProcessor::instance()->processDBusCallNoResponse(CMD_SW_VERSION_REQUEST, msg);
}

void SysInfo::LgeDBus_BTMACAddrStore(const QByteArray &in0){
    QDBusMessage msg;
    for (int i = 0; i < in0.count(); ++i)
        msg << in0.at(i);
    CMsgProcessor::instance()->processDBusCallNoResponse(CMD_BT_MAC_ADDR_STORE, msg);
}

QByteArray SysInfo::LgeDBus_AskBTMACAddr(){
    setDelayedReply(true);
    CMsgProcessor::instance()->processDBusCallWithResponse(CMD_ASK_BT_MAC_ADDR,
                                               connection(), message().createReply());
    return QByteArray();
}

QByteArray SysInfo::LgeDBus_AskAMPVersion(){
    setDelayedReply(true);
    CMsgProcessor::instance()->processDBusCallWithResponse(CMD_ASK_AMP_VERSION,
                                               connection(), message().createReply());
    return QByteArray();
}

bool SysInfo::handleEvent(quint16 cmd, const QByteArray & data){
    switch(cmd){
    case CMD_ETC_CAR_IF_STATUS:
        if ( data.count() == 2)
            emit LgeDBus_EventCarIFStatus((uchar) data.at(0), (uchar) data.at(1));
        else
            mmusUtil->gf_err(MY_ERR, 
					"SysInfo:handleEvent CMD_ETC_CAR_IF_STATUS arguments problem");
        return true;
    case CMD_ETC_DAY_NIGHT_STATUS:
        if ( data.count())
            emit LgeDBus_EventDayNightStatus((uchar) data.at(0));
        else
            mmusUtil->gf_err(MY_ERR, 
					"SysInfo:handleEvent CMD_ETC_CAR_IF_STATUS arguments problem");
		return true;
	case CMD_ETC_POPUP_EVENT:
		if ( data.count())
			emit LgeDBus_PopUpEvent(data);
		else
			mmusUtil->gf_err(MY_ERR, 
					"SysInfo:handleEvent CMD_ETC_CAR_IF_STATUS arguments problem");
		return true;
	case CMD_AUDIO_MIXING_CH_CTRL:
		if ( data.count())
			emit LgeDBus_AudioMixCHCtrl(data);
		else
			mmusUtil->gf_err(MY_ERR, 
					"SysInfo:handleEvent CMD_AUDIO_MIXING_CH_CTRL arguments problem");
		return true;
	case CMD_AUDIO_MAIN_CH_CTRL:
		if ( data.count())
			emit LgeDBus_AudioMainCHCtrl(data);
		else
			mmusUtil->gf_err(MY_ERR, 
					"SysInfo:handleEvent CMD_AUDIO_MAIN_CH_CTRL arguments problem");
		return true;
	case CMD_AUDIO_LAST_MEMORY:
		if ( data.count())
			emit LgeDBus_AudioLastMemory(data);
		else
			mmusUtil->gf_err(MY_ERR, 
					"SysInfo:handleEvent CMD_AUDIO_LAST_MEMORY arguments problem");
		return true;
	case CMD_AUDIO_MUTE_CTRL:
		if ( data.count())
			emit LgeDBus_AudioMuteCtrl(data);
		else
			mmusUtil->gf_err(MY_ERR, 
					"SysInfo:handleEvent CMD_AUDIO_MUTE_CTRL arguments problem");
		return true;
	}

	return false;
}

