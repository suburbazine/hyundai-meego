#include "osd_seatcontrol.h"
#include "common.h"

OSDSeatControl::OSDSeatControl(QPixmap * pixmap, int variant)
: AbstractOSDDraw(pixmap, variant){
	command.append("SEAT_CONTROL");
	
	modeIcon = new QLabel(this);
	modeIcon->setPixmap(QPixmap(g_osd_mode_icons[e_OSD_SEAT]));
	seatLabel = new QLabel(this);
	textLabel = new QLabel(this);
	setUIPosition();
}

void OSDSeatControl::setUIPosition(){
	if ( isMiddleEast()){
		int startX = 1280 - 18 - OSD_ICON_WH;
		modeIcon->setGeometry(startX, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);

		startX -= 8;
		seatLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight | Qt::AlignAbsolute);
		seatLabel->setGeometry(1280 - 443, OSD_ICON_UPPER_MARGIN, 443 - OSD_ICON_SPACE_WIDTH - 18, OSD_ICON_WH);

		textLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter | Qt::AlignAbsolute);
		textLabel->setGeometry(0, OSD_ICON_UPPER_MARGIN, 1280 - 443, OSD_ICON_WH);	
	}
	else{
		int startX = 18;
		modeIcon->setGeometry(startX, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);
		startX += OSD_MODE_ICON_SPACE_WIDTH;
		
		seatLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		seatLabel->setGeometry(startX, OSD_ICON_UPPER_MARGIN, 443 - startX, OSD_ICON_WH);

		textLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		textLabel->setGeometry(443, OSD_ICON_UPPER_MARGIN, 619, OSD_ICON_WH);
	}
}

bool OSDSeatControl::processCommand(const QStringList & args){
	seatLabel->setText(getSubCommand());
	textLabel->setText(args.at(0));

	return true;
}

int OSDSeatControl::getMinimumArgCount(){
	return 3;
}

const QStringList & OSDSeatControl::getCommand(){
	return command;
}
