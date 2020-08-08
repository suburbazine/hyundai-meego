#include "lcd.h"
#include "protocol_common.h"

Lcd::Lcd(QObject * obj) : ObjectAbstractInterface(obj){
    adaptor = new LcdAdaptor(this);
	mmusUtil = MMusUtil::instance();
}

QByteArray Lcd::LgeDBus_AskLcdSetInfoValue(){
    setDelayedReply(true);
    CMsgProcessor::instance()->processDBusCallWithResponse(CMD_ASK_LCD_SET_INFO_VALUE,
                                               connection(), message().createReply());
    return QByteArray();
}

void Lcd::LgeDBus_SetBacklightControl(uchar in0){
    QDBusMessage msg;
    msg << in0;
    CMsgProcessor::instance()->processDBusCallNoResponse(CMD_HW_LCD_BACKLIGHT_CONTROL, msg);
}

void Lcd::LgeDBus_SetLcdPWMControl(uchar in0, uchar in1, uchar in2, uchar in3){
    QDBusMessage msg;
    msg << in0 << in1 << in2 << in3;
    CMsgProcessor::instance()->processDBusCallNoResponse(CMD_HW_LCD_PWM_CONTROL, msg);
}

bool Lcd::handleEvent(quint16 cmd, const QByteArray & data){
	switch(cmd){
		case CMD_LCD_SETUP_INFO:
			if ( data.count() > 0)
				emit LgeDBus_EventLCDSetupInfo(data);
			else
				mmusUtil->gf_err(MY_ERR, 
						"Lcd:handleEvent CMD_LCD_SETUP_INFO arguments problem");
			return true;
	}

	return false;  
}
