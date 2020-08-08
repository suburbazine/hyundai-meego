#include "osd_dmb.h"
#include "common.h"
#include "statusicon.h"
#include "osdview.h"


OSDDmb::OSDDmb(QPixmap * pixmap, int variant)
: AbstractOSDDraw(pixmap, variant){
	command.append("DMB");

	bandLabel = new QLabel(this);
	modeIcon = new QLabel(this);
	modeIcon->setPixmap(QPixmap(g_osd_mode_icons[e_OSD_DMB]));
	infoIcon = new QLabel(this);

	channelLabel = new QLabel(this);
	dialPalette = channelLabel->palette();	
	normalPalette = dialPalette;
	dialPalette.setColor(QPalette::WindowText, OSDView::DialColor());
	normalPalette.setColor(QPalette::WindowText, OSDView::BrightGrey());

	statusLabel = new QLabel(this);
	QPalette pal = statusLabel->palette();
	pal.setColor(QPalette::WindowText, QColor(255, 0, 0));
	statusLabel->setPalette(pal);

	statusIcon = new StatusIcon(this);

	// ignore ME Variant 
	if ( !isMiddleEast()){
		int startX = 18;
		modeIcon->setGeometry(startX, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);
		startX += OSD_MODE_ICON_SPACE_WIDTH;

		bandLabel->setGeometry(startX, OSD_ICON_UPPER_MARGIN, 314, OSD_ICON_WH);
		bandLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);	

		startX = OSD_SUB_ICON_X;
		infoIcon->setGeometry(startX, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);

		startX += OSD_ICON_SPACE_WIDTH;

		channelLabel->setGeometry(startX, OSD_ICON_UPPER_MARGIN, 1036 - startX, OSD_ICON_WH);
		channelLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
		statusLabel->setGeometry(925, OSD_ICON_UPPER_MARGIN, 330, OSD_ICON_WH);
		statusLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
		statusIcon->setGeometry(1131 + 83, 20, 53, 53);

		statusLabel->hide();
		statusIcon->hide();
	}
}

void OSDDmb::setUIPosition(){
	// ignore
}

bool OSDDmb::processCommand(const QStringList & args){
	QStringList dmbInfo;
	dmbInfo.append(args);

	int startX = OSD_SUB_ICON_X;
	if ( getSubCommand() == "WARN")
		infoIcon->setPixmap(QPixmap(g_osd_info_icons[e_INFO_INFO]));
	else
		infoIcon->setPixmap(QPixmap(g_osd_info_icons[e_INFO_CH]));

	infoIcon->setGeometry(startX, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);
	startX += OSD_ICON_SPACE_WIDTH;
	infoIcon->show();

	channelLabel->setGeometry(startX, OSD_ICON_UPPER_MARGIN, 1036 - startX, OSD_ICON_WH);

	bool loadingFound = false;
	bool scanFound = false;
	for (int i = 0; i < dmbInfo.count(); ++i){
		if ( StatusIcon::IconIndex(dmbInfo.at(i)) == e_OSD_STATUS_LOADING){
			loadingFound = true;
			dmbInfo.removeAt(i);
			--i;
		}
		else if ( StatusIcon::IconIndex(dmbInfo.at(i)) == e_OSD_STATUS_SCAN){
			scanFound = true;
			dmbInfo.removeAt(i);
			--i;
		}
	}
	
	if ( loadingFound ){
		statusIcon->setIconString("(LO)");
		statusIcon->show();		
	}

	if ( scanFound ){
		statusIcon->setIconString("(SE)");
		statusIcon->show();
	}

	if ( !loadingFound && !scanFound)
		statusIcon->hide();

	bandLabel->setText(dmbInfo.at(0));
	channelLabel->setText(dmbInfo.at(1));

	if ( getSubCommand() == "INFO_DIAL")
		channelLabel->setPalette(dialPalette);
	else
		channelLabel->setPalette(normalPalette);

	if ( getSubCommand() == "INFO_WARN" && dmbInfo.count() == 3){
		statusLabel->setText(dmbInfo.at(2));	
		statusLabel->show();
	}
	else{
		statusLabel->hide();
	}
	return true;
}

int OSDDmb::getMinimumArgCount(){
	return 4;
}

const QStringList & OSDDmb::getCommand(){
	return command;
}
