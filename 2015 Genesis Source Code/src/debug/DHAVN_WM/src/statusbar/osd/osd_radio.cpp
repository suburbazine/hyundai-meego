#include "osd_radio.h"
#include "common.h"
#include "statusicon.h"
#include "osdview.h"


OSDRadio::OSDRadio(QPixmap * pixmap, int variant)
: AbstractOSDDraw(pixmap, variant){
	command.append("RADIO");
	command.append("SXM");
	command.append("HDRADIO");
	command.append("RDS");

	searchIcon = new StatusIcon(this);
	searchIcon->setIconString("(SE)");
	autoStoreIcon = new StatusIcon(this);
	autoStoreIcon->setIconString("(AS)");
	hdIcon = new StatusIcon(this);
	hdIcon->setIconString("(HD)");
	hdChannelLabel = new QLabel(this);
	hdChannelLabel->setAlignment(Qt::AlignLeft);

	infoIcon = new QLabel(this);
	infoIcon->setPixmap(QPixmap(g_osd_info_icons[e_INFO_CH]));

	searchIcon->hide();
 	autoStoreIcon->hide();
	hdIcon->hide();
	hdChannelLabel->hide();

	bandLabel = new QLabel(this);
	modeIcon = new QLabel(this);
	modeIcon->setPixmap(QPixmap(g_osd_mode_icons[e_OSD_RADIO]));

	frequencyLabel = new QLabel(this);
	dialPalette = frequencyLabel->palette();
	normalPalette = dialPalette;
	dialPalette.setColor(QPalette::WindowText, OSDView::DialColor());
	normalPalette.setColor(QPalette::WindowText, OSDView::BrightGrey());

	stationLabel = new QLabel(this);
	QPalette pal = stationLabel->palette();
	pal.setColor(QPalette::WindowText, OSDView::Grey());
	stationLabel->setPalette(pal);

	statusLabel = new QLabel(this);
    m_palette = statusLabel->palette();
	setUIPosition();
}

void OSDRadio::setUIPosition(){
	if ( isMiddleEast()){
		searchIcon->setGeometry(13, 20, 53, 53);
		autoStoreIcon->setGeometry(13, 20, 53, 53);
		hdIcon->setGeometry(99, 20, 53, 53);
		hdChannelLabel->setGeometry(74, 22, 53, 53);
	}
	else{
		searchIcon->setGeometry(1131 + 83, 20, 53, 53);
		autoStoreIcon->setGeometry(1131 + 83, 20, 53, 53);
		hdIcon->setGeometry(1131, 20, 53, 53);
		hdChannelLabel->setGeometry(1184, 22, 53, 53);
	}


	if ( isMiddleEast()){
		int startX = 178;
		stationLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight | Qt::AlignAbsolute);
		startX += 445 + OSD_ICON_SPACE_WIDTH + 20 - 75;
		frequencyLabel->setGeometry(startX, OSD_ICON_UPPER_MARGIN, 110 + 25, OSD_ICON_WH);
		frequencyLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

		startX = 784;
		infoIcon->setGeometry(startX, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);

		startX = 784 + OSD_ICON_WH + 15;

		bandLabel->setGeometry(startX, OSD_ICON_UPPER_MARGIN, 314, OSD_ICON_WH);
		bandLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight | Qt::AlignAbsolute);
		startX += 314 + 8;
		modeIcon->setGeometry(startX, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);
		statusLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	}
	else{
		int startX = 18;
		modeIcon->setGeometry(startX, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);
		startX += OSD_MODE_ICON_SPACE_WIDTH;

		bandLabel->setGeometry(startX, OSD_ICON_UPPER_MARGIN, 314, OSD_ICON_WH);
		bandLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);	

		startX = OSD_SUB_ICON_X;
		infoIcon->setGeometry(startX, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);
		startX += OSD_ICON_SPACE_WIDTH;	

		stationLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

		if ( getVariant() == 0)
			frequencyLabel->setGeometry(startX, OSD_ICON_UPPER_MARGIN, 98 + 25, OSD_ICON_WH);
		else
			frequencyLabel->setGeometry(startX, OSD_ICON_UPPER_MARGIN, 110 + 25, OSD_ICON_WH);

		frequencyLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

		statusLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	}

}

const QStringList & OSDRadio::getCommand(){
	return command;
}

bool OSDRadio::processCommand(const QStringList & args){
	QStringList radioInfo;
	radioInfo.append(args);

	statusLabel->hide();
	
	if ( getSubCommand() == "INFO")
		frequencyLabel->setPalette(normalPalette);
	else if ( getSubCommand() == "INFO_DIAL")
		frequencyLabel->setPalette(dialPalette);

	bool scanFound = false;
	bool hdFound = false;
	bool hdChannelFound = false;
	bool advisory = false;
	bool autoStroeFound = false;
	for (int i = 0; i < radioInfo.count(); ++i){
		if ( StatusIcon::IconIndex(radioInfo.at(i)) == e_OSD_STATUS_SCAN){
			scanFound = true;
			radioInfo.removeAt(i);
			--i;
                } else if ( StatusIcon::IconIndex(radioInfo.at(i)) == e_OSD_STATUS_HD){
			hdFound = true;
			radioInfo.removeAt(i);
			--i;
                } else if (StatusIcon::IconIndex(radioInfo.at(i)) == e_OSD_STATUS_AUTOSTORE) {
                        autoStroeFound = true;
                        radioInfo.removeAt(i);
                        --i;
                } else if ( radioInfo.at(i).startsWith("(") && radioInfo.at(i).endsWith(")")){
			int ch;
			QString chNum = radioInfo.at(i);
			chNum.remove(0, 1);
			chNum.remove(chNum.length() - 1, 1);
			ch = chNum.toInt(&hdChannelFound);
			if ( hdChannelFound){
				QPalette hdpal = hdChannelLabel->palette();
				hdpal.setColor(QPalette::WindowText, OSDView::HDChanelColor());
				hdChannelLabel->setPalette(hdpal);

				hdChannelLabel->setText(QString::number(ch));
				radioInfo.removeAt(i);
				--i;
			}
		}
	}

	if ( radioInfo.count() < 4){
		if ( scanFound )
			searchIcon->show();
		else
			searchIcon->hide();

		if ( autoStroeFound )
			autoStoreIcon->show();
		else
			autoStoreIcon->hide();
			
		if ( getVariant() == 1){
			if ( hdFound )
				hdIcon->show();
			else
				hdIcon->hide();

			if ( hdChannelFound )
				hdChannelLabel->show();
			else
				hdChannelLabel->hide();
		}
	}

	bandLabel->setText(radioInfo.at(0));
	frequencyLabel->setText(radioInfo.at(1));
	
	if ( radioInfo.count() > 2){
		if ( radioInfo.count() > 3){

                        QString strRadioAdvisory;
                        if(radioInfo.at(3) == "ADVISORY") {
                            advisory = true;
                        } else {
                            advisory = false;
                        }

                        if(advisory) {
                            strRadioAdvisory = radioInfo.at(4);
                        } else {
                            strRadioAdvisory = radioInfo.at(3);
                        }

                        int warnWidth = AbstractOSDDraw::GetStringWidth(font(), strRadioAdvisory);

                        //dmchoi [ITS 0220333] : 미가입 된 SXM Radio 화면 중 Sacn All Channels SK > Home SK 입력 시 OSD 문구와 돋보기 Icon 겹쳐져 출력
                        if(!scanFound) {
                            searchIcon->hide();
                        }

                        if (!hdFound) {
                            hdIcon->hide();
                        }
                        if(!autoStroeFound) {
                            autoStoreIcon->hide();
                        }
                        //dmchoi [ITS 0220333]

			if ( isMiddleEast()){
				statusLabel->setGeometry( 25, OSD_ICON_UPPER_MARGIN, warnWidth, OSD_ICON_WH);
				stationLabel->setGeometry( 25 + warnWidth + 15, OSD_ICON_UPPER_MARGIN, (178 + 445 - (25 + warnWidth + 15)), OSD_ICON_WH);
			}
			else{
				statusLabel->setGeometry(1255 - warnWidth, OSD_ICON_UPPER_MARGIN, warnWidth, OSD_ICON_WH);
				int stationStartX = OSD_SUB_ICON_X + OSD_ICON_SPACE_WIDTH + 140;
				stationLabel->setGeometry( stationStartX, OSD_ICON_UPPER_MARGIN, (1280 - 25 - (warnWidth + 15) - stationStartX), OSD_ICON_WH);
			}

                        if(advisory) {
                             m_palette.setColor(QPalette::WindowText, OSDView::Grey());
                        } else {
                             m_palette.setColor(QPalette::WindowText, QColor(255, 0, 0));
                        }
                        statusLabel->setPalette(m_palette);
                        statusLabel->setText(fitStringToWidget(strRadioAdvisory, statusLabel));
			statusLabel->show();
		}
		else{
			if ( isMiddleEast()){
				stationLabel->setGeometry(178, OSD_ICON_UPPER_MARGIN, 445 + OSD_ICON_SPACE_WIDTH - 75, OSD_ICON_WH);
			}
			else{
				stationLabel->setGeometry(OSD_SUB_ICON_X + OSD_ICON_SPACE_WIDTH + 98 + 25 + 17, OSD_ICON_UPPER_MARGIN, 444, OSD_ICON_WH);
			}
		}

		stationLabel->setText(fitStringToWidget(radioInfo.at(2), stationLabel));
	}
	else{
		stationLabel->clear();
	}

	return true;
}

int OSDRadio::getMinimumArgCount(){
	return 4;
}

