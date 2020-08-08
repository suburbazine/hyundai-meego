#include "osd_volume.h"
#include "common.h"
#include "osdview.h"
#include "osdtextwidget.h"
#include "dhavnlog.h"

OSDVolume::OSDVolume(QPixmap * pixmap, int variant)
: AbstractOSDDraw(pixmap, variant){
	command.append("VOLUME");

	iconLabel = new QLabel(this);
	textLabel = new OSDTextWidget(this);
	volPixLabel = new QLabel(this);
	volLabel = new OSDTextWidget(this);
	volLabel->setAlignment(Qt::AlignCenter);

	for (int i = 0; i < MaxVolumeStep; ++i){
		QString suffix = QString::number(i);
		if ( suffix.length() < 2)
			suffix.prepend("0");

		volPixmaps[i] = new QPixmap(QString(":images/osd/vol/vol_level_") + suffix + ".png");
	}

	iconLabel->show();
	textLabel->show();
	volLabel->show();
	volPixLabel->show();
}

void OSDVolume::setUIPosition(){
	// runtime ui pos
}

const QStringList & OSDVolume::getCommand(){
	return command;
}

bool OSDVolume::processCommand(const QStringList & args){
	int iconIndex = -1;
	int volume = -1;

	if ( getSubCommand() == "VOLUME")
		iconIndex = e_INFO_SOUND;
	else if ( getSubCommand() == "VOLUME_MUTE")
		iconIndex = e_INFO_MUTE;

	if ( iconIndex < 0)
		return false;

	iconLabel->setPixmap(QPixmap(g_osd_info_icons[iconIndex]));

	if ( isMiddleEast()){
                textLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter | Qt::AlignAbsolute);
                textLabel->setLayoutDirection(Qt::RightToLeft); //dmchoi
		textLabel->setGeometry(842 + 88, OSD_ICON_UPPER_MARGIN, 340, OSD_ICON_WH);
		iconLabel->setGeometry(842, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);
		volLabel->setGeometry(266 + 503, OSD_ICON_UPPER_MARGIN, 80, OSD_ICON_WH);
                volLabel->setLayoutDirection(Qt::RightToLeft); //dmchoi
		volPixLabel->setGeometry(266, 19, 494, 46);
	}
	else{
		textLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
                textLabel->setLayoutDirection(Qt::LeftToRight); //dmchoi
		textLabel->setGeometry(10, OSD_ICON_UPPER_MARGIN, 340, OSD_ICON_WH);
		iconLabel->setGeometry(370, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);
		volLabel->setGeometry(431, OSD_ICON_UPPER_MARGIN, 80, OSD_ICON_WH);
                volLabel->setLayoutDirection(Qt::LeftToRight); //dmchoi
		volPixLabel->setGeometry(520, 19, 494, 46);
	}

	textLabel->setText(args.at(0));

	volume = args.at(1).toInt();

	if ( volume >= MaxVolumeStep - 1){
		volLabel->setText("MAX");
		volPixLabel->setPixmap(*volPixmaps[MaxVolumeStep - 1]);
	}
	else if ( volume >= 0){
		volLabel->setText(args.at(1));
		volPixLabel->setPixmap(*volPixmaps[volume]);
	}
	return true;
}

int OSDVolume::getMinimumArgCount(){
	return 2;
}
