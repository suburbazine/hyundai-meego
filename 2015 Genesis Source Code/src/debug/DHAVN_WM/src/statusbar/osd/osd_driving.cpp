#include "osd_driving.h"
#include "common.h"

#define DRIVING_TOP_MARGIN		5
#define DRIVING_IMAGE_WIDTH		180
#define DRIVING_IMAGE_HEIGHT	84
#define DRIVING_TITLE_WIDTH		466

#define DRIVING_IMAGE_ALL_WIDTH	196
#define DRIVING_TEXT_WIDTH		230

extern int getCDC();

OSDDriving::OSDDriving(QPixmap * pixmap, int variant)
: AbstractOSDDraw(pixmap, variant){
	command.append("DRIVE");

	modeIcon = new QLabel(this);
	driveLabel = new QLabel(this);
	driveIcon = new QLabel(this);
	textLabel = new QLabel(this);
	setUIPosition();
}

void OSDDriving::setUIPosition(){
	if ( isMiddleEast()){
		int startX = 1280 - 18 - OSD_ICON_WH;
		modeIcon->setGeometry(startX, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);
		
		startX -= 8;
		startX -= 314;
		driveLabel->setGeometry(startX, OSD_ICON_UPPER_MARGIN, 314, OSD_ICON_WH);
		driveLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter | Qt::AlignAbsolute);
/*	
		startX -= (15 - 2);
		//sepPixmap->setGeometry(startX, 8, 2, 76);

		startX = 1280 - OSD_SUB_ICON_X;
		driveIcon->setGeometry(1280 - DRIVING_TITLE_WIDTH - DRIVING_IMAGE_WIDTH, DRIVING_TOP_MARGIN, DRIVING_IMAGE_WIDTH, DRIVING_IMAGE_HEIGHT);
		textLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
		textLabel->setGeometry(1280 - DRIVING_TITLE_WIDTH - DRIVING_IMAGE_WIDTH - DRIVING_TEXT_WIDTH, OSD_ICON_UPPER_MARGIN, DRIVING_TEXT_WIDTH, OSD_ICON_WH);*/
	}
	else{
		int startX = 18;
		modeIcon->setGeometry(startX, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);
		startX += OSD_MODE_ICON_SPACE_WIDTH;
		
		driveLabel->setGeometry(startX, OSD_ICON_UPPER_MARGIN, 314, OSD_ICON_WH);
		driveLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		//sepPixmap->setGeometry(421, 8, 2, 76);
/*
		startX = OSD_SUB_ICON_X;
		driveIcon->setGeometry(DRIVING_TITLE_WIDTH, DRIVING_TOP_MARGIN, DRIVING_IMAGE_WIDTH, DRIVING_IMAGE_HEIGHT);
		textLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		textLabel->setGeometry(DRIVING_TITLE_WIDTH + DRIVING_IMAGE_ALL_WIDTH, OSD_ICON_UPPER_MARGIN, DRIVING_TEXT_WIDTH, OSD_ICON_WH);
*/
	}
}

bool OSDDriving::processCommand(const QStringList & args){
	bool isME = isMiddleEast();

	if ( getSubCommand() == "HEIGHT"){
		modeIcon->setPixmap( QPixmap(g_osd_mode_icons[e_OSD_HEIGHT]));


		if ( args.at(1) == "HIGH"){
			if ( isME )
				driveIcon->setPixmap(QPixmap(":images/osd/me_ico_drive_high.png"));
			else
				driveIcon->setPixmap(QPixmap(":images/osd/ico_drive_high.png"));
		}
		else if ( args.at(1) == "MIDDLE"){
			if ( isME)
				driveIcon->setPixmap(QPixmap(":images/osd/me_ico_drive_middle.png"));
			else
				driveIcon->setPixmap(QPixmap(":images/osd/ico_drive_middle.png"));
		}
		else if ( args.at(1) == "LOW"){
			if ( isME)
				driveIcon->setPixmap(QPixmap(":images/osd/me_ico_drive_low.png"));
			else
				driveIcon->setPixmap(QPixmap(":images/osd/ico_drive_low.png"));
		}
	}
	else if ( getSubCommand() == "MODE"){
		modeIcon->setPixmap( QPixmap(g_osd_mode_icons[e_OSD_DRIVE]));

		if ( args.at(1) == "NORMAL"){
                        if ( isME){
                            if (getCDC() == 0) {
                                driveIcon->setPixmap(QPixmap(":images/osd/me_ico_drive_normal_noecs.png"));
                            } else {
                                driveIcon->setPixmap(QPixmap(":images/osd/me_ico_drive_normal.png"));
                            }
                       }
                        else {
                            if (getCDC() == 0) {
                                driveIcon->setPixmap(QPixmap(":images/osd/ico_drive_normal_noecs.png"));
                            }
                            else {
                                driveIcon->setPixmap(QPixmap(":images/osd/ico_drive_normal.png"));
                            }

                        }
		}
		else if ( args.at(1) == "SNOW"){
                        if ( isME) {
                            if (getCDC() == 0) {
                                driveIcon->setPixmap(QPixmap(":images/osd/me_ico_drive_snow_noecs.png"));
                            } else {
                                driveIcon->setPixmap(QPixmap(":images/osd/me_ico_drive_snow.png"));
                            }
                       }
                        else {
                             if (getCDC() == 0) {
                                driveIcon->setPixmap(QPixmap(":images/osd/ico_drive_snow_noecs.png"));
                             }
                             else {
                                driveIcon->setPixmap(QPixmap(":images/osd/ico_drive_snow.png"));
                             }
                        }
		}
		else if ( args.at(1) == "ECO"){
                        if ( isME) {
                             if (getCDC() == 0) {
                                 driveIcon->setPixmap(QPixmap(":images/osd/me_ico_drive_eco_noecs.png"));
                             } else {
                                 driveIcon->setPixmap(QPixmap(":images/osd/me_ico_drive_eco.png"));
                             }
                        }
                        else {
                             if (getCDC() == 0) {
                                driveIcon->setPixmap(QPixmap(":images/osd/ico_drive_eco_noecs.png"));
                             }
                             else {
                                driveIcon->setPixmap(QPixmap(":images/osd/ico_drive_eco.png"));
                             }
                        }
		}
		else if ( args.at(1) == "SPORTS"){
                        if ( isME){
                            if (getCDC() == 0) {
                                driveIcon->setPixmap(QPixmap(":images/osd/me_ico_drive_sport_noecs.png"));
                            } else {
                                driveIcon->setPixmap(QPixmap(":images/osd/me_ico_drive_sport.png"));
                            }
                       }
                        else {
                             if (getCDC() == 0) {
                                driveIcon->setPixmap(QPixmap(":images/osd/ico_drive_sport_noecs.png"));
                             }
                             else {
                                driveIcon->setPixmap(QPixmap(":images/osd/ico_drive_sport.png"));
                             }
                        }
		}
	}

	if ( isME ){
		if ( getSubCommand() == "HEIGHT"){
			driveIcon->setGeometry(574, DRIVING_TOP_MARGIN, 230, DRIVING_IMAGE_HEIGHT);
			textLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter | Qt::AlignAbsolute);
			textLabel->setGeometry(334, OSD_ICON_UPPER_MARGIN, 230, OSD_ICON_WH);
		}
		else if ( getSubCommand() == "MODE"){
			driveIcon->setGeometry(634, DRIVING_TOP_MARGIN, DRIVING_IMAGE_WIDTH, DRIVING_IMAGE_HEIGHT);
			textLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter | Qt::AlignAbsolute);
			textLabel->setGeometry(388, OSD_ICON_UPPER_MARGIN, DRIVING_TEXT_WIDTH, OSD_ICON_WH);
		}
	}
	else{
		if ( getSubCommand() == "HEIGHT"){
			driveIcon->setGeometry(DRIVING_TITLE_WIDTH, DRIVING_TOP_MARGIN, 240, DRIVING_IMAGE_HEIGHT);
			textLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
			textLabel->setGeometry(DRIVING_TITLE_WIDTH + 250, OSD_ICON_UPPER_MARGIN, DRIVING_TEXT_WIDTH, OSD_ICON_WH);

		}
		else if ( getSubCommand() == "MODE"){
			driveIcon->setGeometry(DRIVING_TITLE_WIDTH, DRIVING_TOP_MARGIN, DRIVING_IMAGE_WIDTH, DRIVING_IMAGE_HEIGHT);
			textLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
			textLabel->setGeometry(DRIVING_TITLE_WIDTH + DRIVING_IMAGE_ALL_WIDTH, OSD_ICON_UPPER_MARGIN, DRIVING_TEXT_WIDTH, OSD_ICON_WH);
		}
	}

	driveLabel->setText(args.at(0));
	textLabel->setText(args.at(2));

	return true;
}

int OSDDriving::getMinimumArgCount(){
	return 5;
}

const QStringList & OSDDriving::getCommand(){
	return command;
}
