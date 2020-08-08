#include "osd_climate.h"
#include "common.h"
#include "osdview.h"
#include <QTimer>

#include "dhavnlog.h"

OSDClimate::ClimateInfo::ClimateInfo(){
	mainBlower = -1;
	air = -1;
	isBlinking = dualTemp = dualMode = false;
	driverMode = passMode = UNKNOWN;
}

void OSDClimate::setUIPosition(){
	// ignore language change
}

void OSDClimate::driverBlinking(){
	if ( driverModeIcon->isVisible())
		driverModeIcon->hide();
	else
		driverModeIcon->show();

	driverTimerIndex++;
	if ( driverTimerIndex == 6){
		resetTimer();
		driverModeIcon->show();
	}
}

void OSDClimate::passengerBlinking(){
	if ( passengerModeIcon->isVisible())
		passengerModeIcon->hide();
	else
		passengerModeIcon->show();

	passengerTimerIndex++;
	if ( passengerTimerIndex == 6){
		resetTimer(false);
		passengerModeIcon->show();
	}
}

void OSDClimate::resetTimer(bool isFront){
	if ( isFront ){
		driverTimer->stop();
		driverTimerIndex = 0;
	}
	else{
		passengerTimer->stop();
		passengerTimerIndex = 0;
	}
}

void OSDClimate::ClimateInfo::setBlinking(const QString & str){
	if ( str == "1"){
		isBlinking = true;	
	}
	else{
		isBlinking = false;
	}
}

void OSDClimate::ClimateInfo::setAir(const QString & str){
	if ( str == "OFF")
		air = 0;
	else if ( str == "CLEAN")
		air = 1;
	else if ( str == "ION")
		air = 2;
	else if ( str == "SMART VENT")
		air = 3;
	else
		air = -1;	
}

// mode (0 -driver, 1-passenger, 2-ambient
void OSDClimate::ClimateInfo::setTemperature(const QString & str, int type){
	QStringList list = str.split("/");
	if ( list.count() == 2){
                if (type == 0){
			driverStr = list.at(0);
			driverTempStr = list.at(1);
		}
                else if (type == 1){
			passStr = list.at(0);
			passTempStr = list.at(1);
                }
                else {
                    ambientStr = list.at(0);
                    ambientTempStr = list.at(1);
                }
	}
}

void OSDClimate::ClimateInfo::setMode(const QString & str, bool isDriver){
	OSDClimate::ClimateInfo::Mode mode = UNKNOWN;
	if ( str == "OFF")
		mode = OFF;
	else if ( str == "VENT")
		mode = VENT;
	else if ( str == "BL")
		mode = BL;
	else if ( str == "FLOOR")
		mode = FLOOR;
	else if ( str == "MIX")
		mode = MIX;
	else if ( str == "DEF")
		mode = DEF;
	else if ( str == "AUTO_DEF")
		mode = AUTO_DEF;

	if ( isDriver)
		driverMode = mode;
	else
		passMode = mode;
}

void OSDClimate::ClimateInfo::setAuto(const QString & str){
	autoStr = str;
}

void OSDClimate::ClimateInfo::setBlower(const QString & str){
	bool ok;
	int num = str.toInt(&ok);
	if ( ok )
		mainBlower = num;
	else
		mainBlower = -1;
}

void OSDClimate::ClimateInfo::setDualTemperature(const QString & str){
	if ( str == "DUAL")
		dualTemp = true;
	else	// SINGLE
		dualTemp = false;
}

void OSDClimate::ClimateInfo::setDualMode(const QString & str){
	if ( str == "DUAL")
		dualMode = true;
	else	// SINGLE
		dualMode = false;
}

OSDClimate::OSDClimate(QPixmap * pixmap, int variant)
: AbstractOSDDraw(pixmap, variant){
	command.append("CLIMATE");
	createOff();
	createClimate();

	driverTimerIndex = 0;
	passengerTimerIndex = 0;

	driverTimer = new QTimer(this);
        driverTimer->setInterval(300);
	passengerTimer = new QTimer(this);
        passengerTimer->setInterval(300);
	connect(driverTimer, SIGNAL(timeout()), SLOT(driverBlinking()));
	connect(passengerTimer, SIGNAL(timeout()), SLOT(passengerBlinking()));

	m_prevPassengerTempWidth = m_prevDriverTempWidth = 0;
}

bool OSDClimate::processCommand(const QStringList & args){
        lprintf("climate processCommand");
	if ( args.at(8) == "1"){		// DIAG MODE
		if ( driverTimer->isActive()) resetTimer();
		if ( passengerTimer->isActive()) resetTimer(false);

		showDiag(args.at(9).toInt());
		setDriverPosition(args.at(10));
	}
	else{
		if ( args.at(7) == "ON"){	// DATC
			ClimateInfo info;
                        info.setTemperature(getSubCommand(), 0);
                        info.setTemperature(args.at(0), 1);
			info.setMode(args.at(1), true);
			info.setMode(args.at(2), false);
			info.setAuto(args.at(3));
			info.setBlower(args.at(4));	
			info.setDualTemperature(args.at(5));
			info.setDualMode(args.at(6));
			info.setAir(args.at(11));

			if ( args.count() >= 13)
				info.setBlinking(args.at(12));
			else
				info.setBlinking("0");

                        info.setTemperature(args.at(13), 2);

			showClimate(&info);
			setDriverPosition(args.at(10), info.isDualMode());
		}
		else{						// OFF
			if ( driverTimer->isActive()) resetTimer();
			if ( passengerTimer->isActive()) resetTimer(false);

			ClimateInfo info;
			info.setMode(args.at(1), true);
			info.setMode(args.at(2), false);
                        info.setBlower(args.at(4));     // smart vent
                        info.setDualMode(args.at(6));
                        info.setAir(args.at(11));       // blower

                        // off 상태라도 smart vent 일 때는 blower 정보 표시
                        if (info.getAir() == 3) {
                            hideClimate();
                            hideDiag();
                            hideOff();

                            airWidget->setAir(info.getAir());
                            airWidget->setBlow(info.getMainBlower());
                            airWidget->show();
                        }
                        else {
                            airWidget->hide();
                            showOff(args.at(7), info.isDualMode());
                        }

                        setDriverPosition(args.at(10), info.isDualMode());

                        if (info.isDualMode()){
				setModeIcon(info.getDriverMode(), driverModeIcon);
				setModeIcon(info.getPassengerMode(), passengerModeIcon);
				driverModeIcon->show();
				passengerModeIcon->show();
                        }
                        else{
                                setModeIcon(info.getDriverMode(), driverModeIcon);
                                driverModeIcon->show();
                                passengerModeIcon->hide();
                        }

                        // OFF 시에도 외부온도 표시(사양 추가)
                        info.setTemperature(args.at(13), 2);

                        ambientLabel->setText(info.getAmbientStr());
                        ambientTemp->setText(info.getAmbientTemperatureStr());

                        ambientLabel->show();
                        ambientTemp->show();
		}
	}
	return true;
}

void OSDClimate::createOff(){
	offIcon = new QLabel(this);
	offText = new QLabel(this);

	offIcon->setPixmap(QPixmap(":images/osd/climate/ico_wind.png"));

	QFont f = offText->font();
        f.setPointSize(38);
	f.setFamily("HDB");
	offText->setFont(f);

	offText->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
}

void OSDClimate::createClimate(){
	QFont f;
	QPalette pal;
	driverLabel = new QLabel(this);
	driverTemp = new QLabel(this);
	passengerLabel = new QLabel(this);
	passengerTemp = new QLabel(this);
        ambientLabel = new QLabel(this);
        ambientTemp = new QLabel(this);

        passengerLabel = new QLabel(this);
        passengerTemp = new QLabel(this);

	driverModeIcon = new QLabel(this);
	passengerModeIcon = new QLabel(this);

	airWidget = new AirWidget(this);
	autoLabel = new QLabel(this);

        for (int i = 0; i < 5; ++i){
		separatorLabel[i] = new QLabel(this);
		separatorLabel[i]->setPixmap(QPixmap(":images/osd/climate/divider.png"));
	}

	f = driverLabel->font();
	f.setFamily("HDR");
 	if(getVariant() == 5) { //유럽향 일 경우만 Font Size 변경.
 	   f.setPointSize(22);
	} else {
	   f.setPointSize(26);
	}
	driverLabel->setFont(f);
	passengerLabel->setFont(f);
        ambientLabel->setFont(f);

	f = driverTemp->font();
	f.setFamily("HDB");
	f.setPointSize(36);
	autoLabel->setFont(f);
	f.setFixedPitch(true);
	driverTemp->setFont(f);
	passengerTemp->setFont(f);
        ambientTemp->setFont(f);

	pal = driverLabel->palette();
	pal.setColor(QPalette::WindowText, OSDView::Grey());
	driverLabel->setPalette(pal);
	passengerLabel->setPalette(pal);
        ambientLabel->setPalette(pal);

	driverLabel->setAlignment(Qt::AlignCenter);
	driverTemp->setAlignment(Qt::AlignRight | Qt::AlignTop);

	passengerLabel->setAlignment(Qt::AlignCenter);
	passengerTemp->setAlignment(Qt::AlignRight | Qt::AlignTop);
	
        ambientLabel->setAlignment(Qt::AlignCenter);
        ambientTemp->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

	autoLabel->setAlignment(Qt::AlignCenter);
}

void OSDClimate::hideOff(){
	offIcon->hide();
	offText->hide();
}

int OSDClimate::getMinimumArgCount(){
	return 14;
}

void OSDClimate::showClimate(ClimateInfo * info){
	hideOff();
	bool driverKeepTimer = false;
	bool passengerKeepTimer = false;

	if ( driverTimer->isActive() && info->getDriverMode() == OSDClimate::ClimateInfo::AUTO_DEF)
		driverKeepTimer = true;
	else
		resetTimer();

	if ( passengerTimer->isActive() && info->getPassengerMode() == OSDClimate::ClimateInfo::AUTO_DEF)
		passengerKeepTimer = true;
	else
		resetTimer(false);


	driverLabel->setText(info->getDriverStr());	
	driverTemp->setText(info->getDriverTemperatureStr());	
	autoLabel->setText(info->getAutoStr());

	if ( info->isDualTemperature()){
		passengerLabel->setText(info->getPassengerStr());
		passengerTemp->setText(info->getPassengerTemperatureStr());
	}
	else{
		passengerLabel->setText(info->getPassengerStr());
		passengerTemp->setText(info->getDriverTemperatureStr());
	}

        ambientLabel->setText(info->getAmbientStr());
        ambientTemp->setText(info->getAmbientTemperatureStr());

	if ( info->isDualMode()){

            lprintf("showClimate getAutoStr() isDualMode() true: %s,variant:%d", qPrintable(info->getAutoStr()), getVariant());

            // 북미/유럽의 경우 공조 Auto 조건에서는 모드정보 표시 하지 않음
            if ((getVariant() == 1 || getVariant() == 5) && info->getAutoStr() == "AUTO")
            {
                driverModeIcon->hide();
                passengerModeIcon->hide();
            }
            else
            {
		if ( setModeIcon(info->getDriverMode(), driverModeIcon) && !driverKeepTimer && info->showBliking()){
			driverTimer->start();
		}		

		if ( setModeIcon(info->getPassengerMode(), passengerModeIcon) && !passengerKeepTimer && info->showBliking()){
			passengerTimer->start();
		}

		if ( !driverKeepTimer )
			driverModeIcon->show();	

		if ( !passengerKeepTimer)
			passengerModeIcon->show();
            }

            for (int i = 0; i < 4; ++i)
                    separatorLabel[i]->show();
            separatorLabel[4]->hide();

            // 북미/유럽의 경우 공조 Auto 조건에서는 풍량정보 표시 하지 않음
            if ((getVariant() == 1 || getVariant() == 5) && info->getAutoStr() == "AUTO")
            {
                lprintf("showClimate dont show blower");
                airWidget->hide();
            }
            else {
                lprintf("showClimate show blower");
                airWidget->setAir(info->getAir());
                airWidget->setBlow(info->getMainBlower());
                airWidget->show();
            }

	}
	else{
            if (passengerTimer->isActive()) resetTimer(false);

            lprintf("showClimate getAutoStr() isDualMode() false: %s,variant:%d", qPrintable(info->getAutoStr()), getVariant());

            // 북미/유럽의 경우 공조 Auto 조건에서는 모드정보 표시 하지 않음
            if ((getVariant() == 1 || getVariant() == 5) && info->getAutoStr() == "AUTO")
            {
                driverModeIcon->hide();
            }
            else
            {
                if (setModeIcon(info->getDriverMode(), driverModeIcon) && !driverKeepTimer && info->showBliking()){
                        driverTimer->start();
                }

                if ( !driverKeepTimer)
                        driverModeIcon->show();
            }

            passengerModeIcon->hide();
            for (int i = 0; i < 5; ++i)
                    separatorLabel[i]->show();

            // 북미/유럽의 경우 공조 Auto 조건에서는 풍량정보 표시 하지 않음
            if ((getVariant() == 1 || getVariant() == 5) && info->getAutoStr() == "AUTO")
            {
                lprintf("showClimate dont show blower");
                airWidget->hide();
            }
            else {
                lprintf("showClimate show blower");
                airWidget->setAir(info->getAir());
                airWidget->setBlow(info->getMainBlower());
                airWidget->show();
            }
	}

	autoLabel->show();
	driverLabel->show();
	driverTemp->show();
	passengerLabel->show();
	passengerTemp->show();
        ambientLabel->show();
        ambientTemp->show();
}

void OSDClimate::showOff(const QString & str, bool isDualMode){
        hideClimate();
        hideDiag();

        int textWidth = AbstractOSDDraw::GetStringWidth(offText->font(), str);
//	int startX = (1280 - textWidth - 73) / 2;
        int startX = 425; // single mode

        if (isDualMode) {
            startX = 523;
        }

        if ( isMiddleEast()){
                offText->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
                offText->setGeometry(startX, 20, textWidth, 56);
                offIcon->setGeometry(1280 - startX - 53, 20, 53, 56);
        }
        else{
                offIcon->setGeometry(startX, 20, 53, 56);
                startX += 74;
                offText->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
                offText->setGeometry(startX, 20, 1280 - startX, 56);
        }

	offText->setText(str);

	offIcon->show();
	offText->show();
}

void OSDClimate::setDriverPosition(const QString & pos, bool isDualMode){
	// Need to consider middle east variant
	int temperatureDriverWidth = AbstractOSDDraw::GetStringWidth(driverTemp->font(), driverTemp->text()) + 10;
	int temperaturePassengerWidth = AbstractOSDDraw::GetStringWidth(passengerTemp->font(), passengerTemp->text()) + 10;

	if ( m_prevPassengerTempWidth == 0)
		m_prevPassengerTempWidth = temperaturePassengerWidth;

	if ( m_prevDriverTempWidth == 0)
		m_prevDriverTempWidth = temperatureDriverWidth;

	if ( qAbs(m_prevDriverTempWidth - temperatureDriverWidth) < 10)
		temperatureDriverWidth = m_prevDriverTempWidth;

	if ( qAbs(m_prevPassengerTempWidth - temperaturePassengerWidth) < 10)
		temperaturePassengerWidth = m_prevPassengerTempWidth;

	m_prevPassengerTempWidth = temperaturePassengerWidth;
	m_prevDriverTempWidth = temperatureDriverWidth;

	lprintf("driver width : %d", temperatureDriverWidth);
	lprintf("passenger width : %d", temperaturePassengerWidth);

        if (!isDualMode){
            lprintf("setDriverPosition single");

            int driverSpaceWidth = (176 - temperatureDriverWidth) / 2;
            int passengerSpaceWidth = (176 - temperaturePassengerWidth) / 2;

            if ( pos == "RHD"){
                driverLabel->setGeometry(196+198+292+198+198+2, 6, 196, 30);
                driverTemp->setGeometry(1280 - 13 - driverSpaceWidth - temperatureDriverWidth, 30, temperatureDriverWidth, 45);

                passengerLabel->setGeometry(10, 6, 176, 30);
                passengerTemp->setGeometry(/*10 + */4 + passengerSpaceWidth, 30, temperaturePassengerWidth, 45);
            }
            else {
                driverLabel->setGeometry(10, 6, 176, 30);
                driverTemp->setGeometry(/*10 + */4+driverSpaceWidth, 30, temperatureDriverWidth, 45);

                passengerLabel->setGeometry(196+198+292+198+198+2, 6, 196, 30);
                passengerTemp->setGeometry(1280 - 13 - passengerSpaceWidth - temperaturePassengerWidth, 30, temperaturePassengerWidth, 45);
            }

            ambientLabel->setGeometry(196+198+292, 6, 196, 30);
            ambientTemp->setGeometry(196+198+292, 30, 196, 45);

            driverModeIcon->setGeometry(425+53+445, 0, 122, 93);

            autoLabel->setGeometry(10+176+22, 0, 176, 93);
            separatorLabel[0]->setGeometry(196, 0, 2, 93);
            separatorLabel[1]->setGeometry(196 + 198, 0, 2, 93);
            separatorLabel[2]->setGeometry(196 + 198 + 292, 0, 2, 93);
            separatorLabel[3]->setGeometry(196 + 198 + 292 + 198, 0, 2, 93);
            separatorLabel[4]->setGeometry(196 + 198 + 292 + 198 + 198, 0, 2, 93);

            airWidget->setGeometry(425, 15, 262, 61);
        }
        else{
            lprintf("setDriverPosition double");

            int driverSpaceWidth = (180 - temperatureDriverWidth) / 2;
            int passengerSpaceWidth = (180 - temperaturePassengerWidth) / 2;

            if ( pos == "RHD"){
                driverLabel->setGeometry(5+180+138+180+274+180+138, 6, 180, 30);
                driverTemp->setGeometry(1280 - 13 - driverSpaceWidth - temperatureDriverWidth, 30, temperatureDriverWidth, 45);

                passengerLabel->setGeometry(5, 6, 180, 30);
                passengerTemp->setGeometry(/*10 + */4 + passengerSpaceWidth, 30, temperaturePassengerWidth, 45);
            }
            else {
                driverLabel->setGeometry(5, 6, 180, 30);
                driverTemp->setGeometry(/*10 + */4 + driverSpaceWidth, 30, temperatureDriverWidth, 45);

                passengerLabel->setGeometry(5+180+138+180+274+180+138, 6, 180, 30);
                passengerTemp->setGeometry(1280 - 13 - passengerSpaceWidth - temperaturePassengerWidth, 30, temperaturePassengerWidth, 45);
            }

            ambientLabel->setGeometry(5+180+138+180+274, 6, 180, 30);
            ambientTemp->setGeometry(5+180+138+180+274, 30, 180, 45);

            autoLabel->setGeometry(5+180+138, 0, 180, 93);

            separatorLabel[0]->setGeometry(316, 0, 2, 93);
            separatorLabel[1]->setGeometry(316 + 192, 0, 2, 93);
            separatorLabel[2]->setGeometry(316 + 192 + 262, 0, 2, 93);
            separatorLabel[3]->setGeometry(316 + 192 + 262 + 192, 0, 2, 93);

            airWidget->setGeometry(523, 15, 262, 61);
            if ( pos == "RHD"){
                    driverModeIcon->setGeometry(189+780, 0, 122, 93);
                    passengerModeIcon->setGeometry(189, 0, 122, 93);
            }
            else{
                passengerModeIcon->setGeometry(189+780, 0, 122, 93);
                driverModeIcon->setGeometry(189, 0, 122, 93);
            }
        }

}

void OSDClimate::showDiag(int code){
	hideClimate();
	hideDiag();	
	hideOff();

//	driverLabel->setText("Driver");

        QString strCode;

        if (code < 10) {
            strCode.append(QString::number(0));
        }
        strCode.append(QString::number(code));

        driverTemp->setText(strCode);

//	driverLabel->show();
	driverTemp->show();
}

void OSDClimate::hideDiag(){
	driverLabel->hide();
	driverTemp->hide();
}

void OSDClimate::hideClimate(){
	driverLabel->hide();
	driverTemp->hide();

	passengerLabel->hide();
	passengerTemp->hide();

	driverModeIcon->hide();
	passengerModeIcon->hide();

	autoLabel->hide();

        for (int i = 0; i < 5; ++i)
		separatorLabel[i]->hide();

	airWidget->hide();

        ambientLabel->hide();
        ambientTemp->hide();
}

bool OSDClimate::setModeIcon(ClimateInfo::Mode mode, QLabel * label){
	switch(mode){
	case OSDClimate::ClimateInfo::VENT:
		label->setPixmap(QPixmap(":images/osd/climate/wind_mode_02.png"));
		break;
	case OSDClimate::ClimateInfo::BL:
		label->setPixmap(QPixmap(":images/osd/climate/wind_mode_03.png"));
		break;
	case OSDClimate::ClimateInfo::FLOOR:
		label->setPixmap(QPixmap(":images/osd/climate/wind_mode_01.png"));
		break;
	case OSDClimate::ClimateInfo::MIX:
		label->setPixmap(QPixmap(":images/osd/climate/ico_mix.png"));
		break;
	case OSDClimate::ClimateInfo::DEF:
		label->setPixmap(QPixmap(":images/osd/climate/ico_def.png"));
		break;
	case OSDClimate::ClimateInfo::AUTO_DEF:
		label->setPixmap(QPixmap(":images/osd/climate/ico_auto_def.png"));
		return true;
	case OSDClimate::ClimateInfo::OFF:
	default:
		label->clear();
		break;	
	}
	return false;
}

const QStringList & OSDClimate::getCommand(){
	return command;
}
