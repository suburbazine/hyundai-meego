#include "osd_tag.h"
#include "common.h"
#include "statusicon.h"
#include "osdview.h"

OSDTagging::OSDTagging(QPixmap * pixmap, int variant)
: AbstractOSDDraw(pixmap, variant){
	command.append("TAGGING");

	bandLabel = new QLabel(this);
	modeIcon = new QLabel(this);
	modeIcon->setPixmap(QPixmap(g_osd_mode_icons[e_OSD_RADIO]));

	// TODO: block info icon temporarily, not specified GUI Guideline

	titleLabel = new QLabel(this);

	descLabel = new QLabel(this);
	QPalette pal = descLabel->palette();
	pal.setColor(QPalette::WindowText, OSDView::Grey());
	descLabel->setPalette(pal);

	sepPixmap = new QLabel(this);
	sepPixmap->setPixmap(*pixmap);

	titleStartX = 0;
}

void OSDTagging::setUIPosition(){
	if ( isMiddleEast()){
		descLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
		descLabel->setGeometry(0, OSD_ICON_UPPER_MARGIN, 322 + 158, OSD_ICON_WH);

		titleLabel->setGeometry(322 + 158 + 24, OSD_ICON_UPPER_MARGIN, 333, OSD_ICON_WH);
		titleLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

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
		startX = OSD_SUB_ICON_X;


		titleStartX = startX + OSD_ICON_SPACE_WIDTH;

		titleLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
		descLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	}

}

const QStringList & OSDTagging::getCommand(){
	return command;
}

bool OSDTagging::processCommand(const QStringList & args){
	if ( getSubCommand() != "INFO")
		return false;

	bandLabel->setText(args.at(0));

	int w = GetStringWidth(titleLabel->font(), args.at(1));
	titleLabel->setGeometry(titleStartX, OSD_ICON_UPPER_MARGIN, w, OSD_ICON_WH);
	int descStartX = titleStartX + w + 17;
	descLabel->setGeometry(descStartX, OSD_ICON_UPPER_MARGIN, 1280 - 17 - descStartX, OSD_ICON_WH);

	titleLabel->setText(args.at(1));
	if ( args.count() > 2){
		descLabel->setText(fitStringToWidget(args.at(2), descLabel));
		descLabel->show();	
	}
	else{
		descLabel->hide();
	}

	return true;
}

int OSDTagging::getMinimumArgCount(){
	return 4;
}

