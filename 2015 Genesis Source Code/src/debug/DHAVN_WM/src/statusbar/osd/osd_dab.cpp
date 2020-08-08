#include "osd_dab.h"
#include "common.h"
#include "statusicon.h"
#include "osdview.h"

OSDDab::OSDDab(QPixmap * pixmap, int variant)
: AbstractOSDDraw(pixmap, variant){
	command.append("DAB");

	searchIcon = new StatusIcon(this);
	searchIcon->setIconString("(SE)");


	searchIcon->hide();

	bandLabel = new QLabel(this);
	modeIcon = new QLabel(this);
	modeIcon->setPixmap(QPixmap(g_osd_mode_icons[e_OSD_RADIO]));

	infoIcon = new QLabel(this);
	infoIcon->setPixmap(QPixmap(g_osd_info_icons[e_INFO_CH]));

	ensembleLabel = new QLabel(this);
	stationLabel = new QLabel(this);

	statusLabel = new QLabel(this);
	QPalette pal = statusLabel->palette();
	pal.setColor(QPalette::WindowText, QColor(255, 0, 0));
	statusLabel->setPalette(pal);

	dialPalette = stationLabel->palette();
	normalPalette = dialPalette;
	dialPalette.setColor(QPalette::WindowText, OSDView::DialColor());
	normalPalette.setColor(QPalette::WindowText, OSDView::Grey());



	setUIPosition();
}

void OSDDab::setUIPosition(){
	searchIcon->setGeometry(1131 + 83, 20, 53, 53);

	if ( isMiddleEast()){
		// ignore me
	}
	else{
		int startX = 18;
		modeIcon->setGeometry(startX, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);
		startX += OSD_MODE_ICON_SPACE_WIDTH;

		bandLabel->setGeometry(startX, OSD_ICON_UPPER_MARGIN, 314, OSD_ICON_WH);
		bandLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);	

		startX = OSD_SUB_ICON_X;
		infoIcon->setGeometry(startX, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);

		statusLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

		startX += OSD_ICON_SPACE_WIDTH;
		stationLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

		ensembleLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	}
}

const QStringList & OSDDab::getCommand(){
	return command;
}

bool OSDDab::processCommand(const QStringList & args){
	QStringList radioInfo;
	radioInfo.append(args);

	statusLabel->hide();
	
	if ( getSubCommand() == "INFO")
		stationLabel->setPalette(normalPalette);
        else if ( getSubCommand() =="INFO_SEARCH"){ 
                stationLabel->setPalette(normalPalette);
                infoIcon->hide();
        }
	else if ( getSubCommand() == "INFO_DIAL")
		stationLabel->setPalette(dialPalette);

	bool scanFound = false;
	for (int i = 0; i < radioInfo.count(); ++i){
		if ( StatusIcon::IconIndex(radioInfo.at(i)) == e_OSD_STATUS_SCAN){
			scanFound = true;
			radioInfo.removeAt(i);
			--i;
		}
	}

	if ( scanFound )
		searchIcon->show();
	else
		searchIcon->hide();

	bandLabel->setText(radioInfo.at(0));

	QString ensemble = radioInfo.at(1);
	if ( radioInfo.count() > 2){
		ensemble.append("/");
		int w = AbstractOSDDraw::GetStringWidth(font(), ensemble);
		ensembleLabel->setText(ensemble);

		int startX = OSD_SUB_ICON_X + OSD_ICON_SPACE_WIDTH;
		int warnWidth = 0;

		ensembleLabel->setGeometry(startX, OSD_ICON_UPPER_MARGIN, w + 10, OSD_ICON_WH);	
		// TODO: temporarily increase width
		if ( radioInfo.count() > 3){
			warnWidth = AbstractOSDDraw::GetStringWidth(font(), radioInfo.at(3));
			stationLabel->setGeometry(startX + w + 10, OSD_ICON_UPPER_MARGIN, (1255 - warnWidth) - startX - w - 10, OSD_ICON_WH);
			statusLabel->setGeometry( 1255 - warnWidth, OSD_ICON_UPPER_MARGIN, warnWidth, OSD_ICON_WH);
			searchIcon->hide();
		}
		else{
			stationLabel->setGeometry(startX + w + 10, OSD_ICON_UPPER_MARGIN, 1214 - startX - w - 10, OSD_ICON_WH);
		}

		stationLabel->setText(fitStringToWidget(radioInfo.at(2), stationLabel));

		if ( radioInfo.count() > 3){
			statusLabel->setText(fitStringToWidget(radioInfo.at(3), statusLabel));
			statusLabel->show();
		}
	}
	else{
		ensembleLabel->setText(ensemble);
		ensembleLabel->setGeometry(OSD_SUB_ICON_X + OSD_ICON_SPACE_WIDTH, OSD_ICON_UPPER_MARGIN, 1255 - OSD_SUB_ICON_X + OSD_ICON_SPACE_WIDTH, OSD_ICON_WH);

		stationLabel->clear();
	}

	return true;
}

int OSDDab::getMinimumArgCount(){
	return 4;
}

