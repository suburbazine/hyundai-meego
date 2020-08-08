#include "osd_common.h"
#include "common.h"
#include "osdview.h"
#include "osdtextwidget.h"
#include "dhavnlog.h"



OSDCommon::OSDCommon(QPixmap * pixmap, int variant)
: AbstractOSDDraw(pixmap, variant){
	iconLabel = new QLabel(this);
	textLabel = new OSDTextWidget(this);

	command.append("COMMON");

	textLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
}

void OSDCommon::setUIPosition(){
	// process runtime ui pos
}

const QStringList & OSDCommon::getCommand(){
	return command;
}

bool OSDCommon::processCommand(const QStringList & args){
	text = args.at(0);

	int iconIndex = -1;

	if ( getSubCommand() == "WARN" || getSubCommand() == "LOW_BATTERY" || getSubCommand() == "BLOCK_APP_CAMERA")
		iconIndex = e_INFO_INFO;
	else if ( getSubCommand() == "VOLUME_MUTE")
		iconIndex = e_INFO_MUTE;
	else if ( getSubCommand() == "SMART_VENTILATION")
		iconIndex = e_INFO_CLIMATE;

//        lprintf("OSDCommon::processCommand getSubCommand : %s", qPrintable(getSubCommand()));
//        lprintf("OSDCommon::processCommand iconIndex : %d", iconIndex);

	if ( getSubCommand() != "INFO" && iconIndex < 0)
		return false;

	int startX;
	QString transText = g_translate(text);
        int textWidth = 0;
	int onlyTextWidth = 0;

        QFont f = font();
        bool containLineFeed = transText.contains("\n");
        int startY = (containLineFeed ? 0 : OSD_ICON_UPPER_MARGIN);
        int height = (containLineFeed ? 93 : OSD_ICON_WH);
        if ( containLineFeed )
                f.setPointSize(30);
        else
                f.setPointSize(34);
	if ( getSubCommand() == "INFO"){
                textWidth = AbstractOSDDraw::GetStringWidth(f, transText);
		if ( textWidth > 1280 )
			textWidth = 1280;
		
		iconLabel->hide();

		startX = (1280 - textWidth) / 2;
	}
	else{
                onlyTextWidth =	AbstractOSDDraw::GetStringWidth(f, transText);

		if ( onlyTextWidth > 1280 - OSD_ICON_SPACE_WIDTH)
			onlyTextWidth = 1280 - OSD_ICON_SPACE_WIDTH;

		textWidth = onlyTextWidth + OSD_ICON_SPACE_WIDTH;

		if ( iconIndex >= 0){
			iconLabel->setPixmap( QPixmap(g_osd_info_icons[iconIndex]));
		}
		else{
			iconLabel->clear();
		}

		startX = (1280 - textWidth) / 2;
	}


	if ( isMiddleEast()){
		textLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter | Qt::AlignAbsolute);
                textLabel->setLayoutDirection(Qt::RightToLeft); //dmchoi
		//startX = 1280 - startX;
		if ( getSubCommand() == "INFO"){
                        textLabel->setGeometry(startX, startY, textWidth, height);
		}
		else{
			startX = 1280 - startX;
                        iconLabel->setGeometry(startX - OSD_ICON_SPACE_WIDTH + 8, startY, OSD_ICON_WH, height);
                        textLabel->setGeometry(startX - textWidth , startY, onlyTextWidth, height);
        	iconLabel->show(); //ITS : 0217687 전석화면 Mute OSD출력하지만 Mute 아이콘 미출력함 
		}
	}
	else{
		textLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
                textLabel->setLayoutDirection(Qt::LeftToRight);//dmchoi
		if ( getSubCommand() == "INFO"){
                        textLabel->setGeometry(startX, startY, 1280 - startX, height);
		}
		else{
                        iconLabel->setGeometry(startX, startY, OSD_ICON_WH, height);
			startX += OSD_ICON_SPACE_WIDTH;

                        textLabel->setGeometry(startX, startY, 1280 - startX, height);
			iconLabel->show();
		}
	}

	textLabel->setText(fitStringToWidget(transText, textLabel));

	return true;
}

int OSDCommon::getMinimumArgCount(){
	return 3;
}

