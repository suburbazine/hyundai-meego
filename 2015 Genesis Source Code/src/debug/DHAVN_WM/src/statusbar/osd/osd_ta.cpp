#include "osd_ta.h"
#include "common.h"
#include "statusicon.h"
#include "osdview.h"

OSDTa::OSDTa(QPixmap * pixmap, int variant)
: AbstractOSDDraw(pixmap, variant){
	command.append("TA");

	bandLabel = new QLabel(this);
	modeIcon = new QLabel(this);
	modeIcon->setPixmap(QPixmap(g_osd_mode_icons[e_OSD_RADIO]));

	trafficLabel = new QLabel(this);

	stationLabel = new QLabel(this);
	QPalette pal = stationLabel->palette();
	pal.setColor(QPalette::WindowText, OSDView::Grey());
	stationLabel->setPalette(pal);

	sepPixmap = new QLabel(this);
	sepPixmap->setPixmap(*pixmap);
}

void OSDTa::setUIPosition(){
	if ( isMiddleEast()){
		stationLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
		stationLabel->setGeometry(0, OSD_ICON_UPPER_MARGIN, 322 + 158, OSD_ICON_WH);

		trafficLabel->setGeometry(322 + 158 + 24, OSD_ICON_UPPER_MARGIN, 409, OSD_ICON_WH);
		trafficLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

		// TODO : need insert separator position

		int startX = 784;
		startX += OSD_ICON_WH + 15;
		bandLabel->setGeometry(startX, OSD_ICON_UPPER_MARGIN, 314, OSD_ICON_WH);
		bandLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
		startX += 314 + 8;
		modeIcon->setGeometry(startX, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);
	}
	else{
		int startX = 18;
		modeIcon->setGeometry(startX, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);
		startX += OSD_MODE_ICON_SPACE_WIDTH;

		bandLabel->setGeometry(startX, OSD_ICON_UPPER_MARGIN, 314, OSD_ICON_WH);
		bandLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);	

		sepPixmap->setGeometry(421, 8, 2, 76);

		trafficLabel->setGeometry(443, OSD_ICON_UPPER_MARGIN, 409, OSD_ICON_WH);
		trafficLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

		stationLabel->setGeometry(443 + 409 + 20, OSD_ICON_UPPER_MARGIN, 1280 - 443 - 409 - 20, OSD_ICON_WH);
		stationLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	}

}

const QStringList & OSDTa::getCommand(){
	return command;
}

bool OSDTa::processCommand(const QStringList & args){
	QStringList taInfo;
	taInfo.append(args);

	
	if ( getSubCommand() != "INFO")
		return false;


	bandLabel->setText(taInfo.at(0));
        //trafficLabel->setText(taInfo.at(1));
        trafficLabel->setText(fitStringToWidget(taInfo.at(1), trafficLabel));
	stationLabel->setText(taInfo.at(2));

	return true;
}

int OSDTa::getMinimumArgCount(){
	return 5;
}

