#include "osd_sxm_info.h"
#include "common.h"
#include "osdview.h"
#include "dhavnlog.h"

#define PLAY_STATUS_WIDTH	300
#define STATION_WIDTH		213
#define DATA_TITLE_WIDTH	400

OSDSxmInfo::OSDSxmInfo(QPixmap * pixmap, int variant)
: AbstractOSDDraw(pixmap, variant){
	command.append("SXM_INFO");
	command.append("DAB_INFO");

	titleLabel = new QLabel(this);
	bandLabel = new QLabel(this);
	modeIcon = new QLabel(this);
	infoIcon = new QLabel(this);

	stationLabel = new QLabel(this);
	statusLabel = new QLabel(this);

	QPalette pal = statusLabel->palette();
	pal.setColor(QPalette::WindowText, OSDView::Grey());
	statusLabel->setPalette(pal);

	titleStartX = 0;

	setUIPosition();
}

void OSDSxmInfo::setUIPosition(){
	if ( isMiddleEast()){
		titleLabel->setGeometry(322 + 158 + 24, OSD_ICON_UPPER_MARGIN, 333, OSD_ICON_WH);
		titleLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight | Qt::AlignAbsolute);

		int startX = 784;
		startX += OSD_ICON_WH + 15;
		bandLabel->setGeometry(startX, OSD_ICON_UPPER_MARGIN, 314, OSD_ICON_WH);
		bandLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight | Qt::AlignAbsolute);
		startX += 314 + 8;
		modeIcon->setGeometry(startX, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);
		stationLabel->setGeometry(13, OSD_ICON_UPPER_MARGIN, 200, OSD_ICON_WH);
		stationLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	}
	else{
		int startX = 18;
		modeIcon->setGeometry(startX, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);
		startX += OSD_MODE_ICON_SPACE_WIDTH;

		bandLabel->setGeometry(startX, OSD_ICON_UPPER_MARGIN, 314, OSD_ICON_WH);
		bandLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);	

		startX = OSD_SUB_ICON_X;

		infoIcon->setGeometry(startX, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);

		titleStartX = startX + OSD_ICON_SPACE_WIDTH;

		titleLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

		stationLabel->setGeometry(1267  - 200, OSD_ICON_UPPER_MARGIN, 200, OSD_ICON_WH);
		stationLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
	}
}

bool OSDSxmInfo::processCommand(const QStringList & args){
	lprintf("SXM_INFO processCommand");
	if ( getSubCommand() == "EPG"){
		modeIcon->setPixmap(QPixmap(g_osd_mode_icons[e_OSD_SXM]));
		infoIcon->setPixmap(QPixmap(g_osd_info_icons[e_INFO_PROGRAM]));
	}
	else if ( getSubCommand() == "FAVORITE"){
		modeIcon->setPixmap(QPixmap(g_osd_mode_icons[e_OSD_SXM]));
		infoIcon->setPixmap(QPixmap(g_osd_info_icons[e_INFO_MUSIC]));
	}
	else if ( getSubCommand() == "DATA"){
		modeIcon->setPixmap(QPixmap(g_osd_mode_icons[e_OSD_SXM_DATA]));
		infoIcon->setPixmap(QPixmap(g_osd_info_icons[e_INFO_INFO]));
	}
	else{
		return false;
	}

	modeIcon->show();
	infoIcon->show();

	bandLabel->setText(fitStringToWidget(args.at(0), bandLabel));
	

	bandLabel->show();

	if ( getSubCommand() == "EPG"){
		QString titleName = args.at(1) + "/";
		int w = GetStringWidth(titleLabel->font(), titleName);
		lprintf("SXM_INFO EPG : %d", w);
		if ( 1280 - titleStartX - STATION_WIDTH - PLAY_STATUS_WIDTH < w )
			w = 1280 - titleStartX - STATION_WIDTH - PLAY_STATUS_WIDTH;

		titleLabel->setGeometry(titleStartX, OSD_ICON_UPPER_MARGIN, w, OSD_ICON_WH);
		titleLabel->setText(fitStringToWidget(titleName, titleLabel));
		titleLabel->show();

		if ( args.count() > 2){
			statusLabel->setGeometry(titleStartX + w + 15, OSD_ICON_UPPER_MARGIN, PLAY_STATUS_WIDTH, OSD_ICON_WH);
			statusLabel->setText(fitStringToWidget(args.at(2), statusLabel));
			statusLabel->show();

			if ( args.count() > 3){
				stationLabel->setText(fitStringToWidget(args.at(3), stationLabel));
				stationLabel->show();
			}
			else{
				stationLabel->hide();
			}
		}
		else{
			statusLabel->hide();
			stationLabel->hide();
		}
		
	}
	else if ( getSubCommand() == "DATA"){
		int w = GetStringWidth(titleLabel->font(), args.at(1));
		if ( w > DATA_TITLE_WIDTH )
			w = DATA_TITLE_WIDTH;

		titleLabel->setGeometry(titleStartX, OSD_ICON_UPPER_MARGIN, w, OSD_ICON_WH);
		titleLabel->setText(fitStringToWidget(args.at(1), titleLabel));
		titleLabel->show();
		
		statusLabel->setGeometry(titleStartX + w + 20, OSD_ICON_UPPER_MARGIN, 1280 - titleStartX - w - 20, OSD_ICON_WH);

		if ( args.count() >= 3){
			statusLabel->setText(fitStringToWidget(args.at(2), statusLabel));
			statusLabel->show();
		}
		else{
			statusLabel->hide();
		}

		stationLabel->hide();
	}
	else{
		int w = GetStringWidth(titleLabel->font(), args.at(1));
		if ( 1280 - titleStartX < w )
			w = 1280 - titleStartX;
		
		titleLabel->setGeometry(titleStartX, OSD_ICON_UPPER_MARGIN, w, OSD_ICON_WH);
		titleLabel->setText(fitStringToWidget(args.at(1), titleLabel));
		titleLabel->show();

		stationLabel->hide();
		statusLabel->hide();
	}
	return true;
}

const QStringList & OSDSxmInfo::getCommand(){
	return command;
}

int OSDSxmInfo::getMinimumArgCount(){
	return 4;
}
