#include "osd_media_info.h"
#include "infoicon.h"
#include "common.h"
#include "osdtextwidget.h"
#include "dhavnlog.h"

OSDMediaInfo::OSDMediaInfo(QPixmap * pixmap, int variant)
: AbstractOSDDraw(pixmap, variant){
	command.append("MEDIA_INFO_MODE");
	command.append("MEDIA_INFO");
	command.append("CONNECT_INFO");
	command.append("CONNECT2_INFO");

	modeIcon = new QLabel(this);
	deviceTypeLabel = new QLabel(this);
        deviceTypeLabel2Line = new OSDTextWidget(this);
	infoIcon = new InfoIcon(this);
	textLabel = new QLabel(this);	
        textLabel2Line = new OSDTextWidget(this);
        startX = 0;

	//sepPixmap = new QLabel(this);
	//sepPixmap->setPixmap(*pixmap);

    //setUIPosition();
}

void OSDMediaInfo::setUIPosition(){
}
void OSDMediaInfo::setUIGeometry(int startY, int height){
	if ( isMiddleEast()){
                startX = 1280 - 18 - OSD_ICON_WH;
		modeIcon->setGeometry(startX, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);
		
		startX -= 8;
		startX -= 314;

        deviceTypeLabel->setGeometry(startX, startY, 314, height);
		deviceTypeLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight | Qt::AlignAbsolute);

        deviceTypeLabel2Line->setGeometry(startX, 0, 314, height);
                deviceTypeLabel2Line->setAlignment(Qt::AlignVCenter | Qt::AlignRight | Qt::AlignAbsolute);
                deviceTypeLabel2Line->setLayoutDirection(Qt::RightToLeft);

		startX -= 15 - 2;
		//sepPixmap->setGeometry(startX, 8, 2, 76);

		startX = 1280 - OSD_SUB_ICON_X - 73;
        infoIcon->setGeometry(startX, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, height);

		startX -= 7;
        textLabel2Line->setGeometry(0, 0, startX, height);
                textLabel2Line->setAlignment(Qt::AlignVCenter | Qt::AlignRight | Qt::AlignAbsolute);
                textLabel2Line->setLayoutDirection(Qt::RightToLeft);
        textLabel->setGeometry(0, startY, startX, height);
		textLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight | Qt::AlignAbsolute);
	}
    else
    {
                startX = 18;
		modeIcon->setGeometry(startX, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);
		startX += OSD_MODE_ICON_SPACE_WIDTH;

        deviceTypeLabel->setGeometry(startX, startY, 314, height);
		deviceTypeLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

        deviceTypeLabel2Line->setGeometry(startX, 0, 314, height);
                deviceTypeLabel2Line->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
                deviceTypeLabel2Line->setLayoutDirection(Qt::LeftToRight);

		//sepPixmap->setGeometry(421, 8, 2, 76);

		startX = OSD_SUB_ICON_X;
        infoIcon->setGeometry(startX, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, height);
		startX += OSD_ICON_SPACE_WIDTH;

        textLabel2Line->setGeometry(startX, 0, 1280 - startX, height);
                textLabel2Line->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
                textLabel2Line->setLayoutDirection(Qt::LeftToRight);
        textLabel->setGeometry(startX , startY, 1280 - startX , height);
		textLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	}
}

bool OSDMediaInfo::processCommand(const QStringList & args){

    QFont font1 = font();
    QFont font2 = font();

    QString transdevice =  g_translate(args.at(1));

    if(args.at(2) == "MULTI_LINE") {
       QString text = g_translate(args.at(3));
       textLabel->setText(g_translate(args.at(3)));

        bool multi_containLineFeed = text.contains("\n");
        int startY = (multi_containLineFeed ? 0 : OSD_ICON_UPPER_MARGIN);
        int height = (multi_containLineFeed ? 93 : OSD_ICON_WH);
        setUIGeometry(startY, height);
        if ( multi_containLineFeed )
            font1.setPointSize(30);
        else
            font1.setPointSize(34);
//        setUIGeometry(startY, height);
             if (text.contains("\n")) {
                setMultiLineGeometry(text);
                textLabel2Line->setText(text);
                textLabel2Line->show();
                textLabel->hide();
            }else{
                if ( args.count() >= 3){
                    textLabel->setText(fitStringToWidget(text, textLabel));
                }
                textLabel->show();
                textLabel2Line->hide();
            }
        } else {

        QString transText =  g_translate(args.at(2));
        bool single_containLineFeed = transText.contains("\n");
        int single_startY = (single_containLineFeed ? 0 : OSD_ICON_UPPER_MARGIN);
        int single_height = (single_containLineFeed ? 88 : OSD_ICON_WH);

        setUIGeometry(single_startY, single_height);
        if ( single_containLineFeed ) {
            font2.setPointSize(30);

            setMultiLineGeometry(transText);
            textLabel2Line->setText(transText);
            textLabel2Line->show();
            textLabel->hide();
        }
        else {
            font2.setPointSize(34);

            if ( args.count() >= 3){
                textLabel->setText(fitStringToWidget(transText, textLabel));
            }
            textLabel->show();
            textLabel2Line->hide();
        }

     }

     deviceTypeLabel->setText(transdevice);
     int deviceTypeLabelWidth = AbstractOSDDraw::GetStringWidth(font2, deviceTypeLabel->text());
     deviceTypeLabel->show();

      modeIcon->setPixmap(QPixmap(g_osd_mode_icons[ g_string_to_media_index(args.at(0))]));

	if ( getSubCommand() == "LOADING")
		infoIcon->setIcon( e_INFO_LOADING);
	else if ( getSubCommand() == "ERR")
		infoIcon->setIcon( e_INFO_DISABLE);
	else if ( getSubCommand() == "WARN")
		infoIcon->setIcon( e_INFO_INFO);
	else if ( getSubCommand() == "THUMBUP")
		infoIcon->setIcon( e_INFO_UP);
	else if ( getSubCommand() == "THUMBDOWN")
		infoIcon->setIcon( e_INFO_DOWN);
	else
		infoIcon->setIcon(-1);

	return true;
}

void OSDMediaInfo::setMultiLineGeometry(QString str) {
    QFont font1 = font();
    lprintf("setMultiLineGeometry >>>> str : %s", qPrintable(str));
    lprintf("setMultiLineGeometry >>>> startX : %d", startX);
    bool multi_containLineFeed = str.contains("\n");
    int startY = (multi_containLineFeed ? 0 : OSD_ICON_UPPER_MARGIN);
    int height = (multi_containLineFeed ? 93 : OSD_ICON_WH);
    if ( multi_containLineFeed )
        font1.setPointSize(28);         // 30
    else
        font1.setPointSize(34);
    lprintf("setMultiLineGeometry >>>> pointSize() : %d",font1.pointSize());
    textLabel2Line->setFont(font1);
    if (isMiddleEast()){
        textLabel2Line->setGeometry(0, startY, startX, height);
    }
    else
    {
        textLabel2Line->setGeometry(startX, startY, 1280 - startX, height);
    }
    return;
}

const QStringList & OSDMediaInfo::getCommand(){
	return command;
}

int OSDMediaInfo::getMinimumArgCount(){
	return 5;
}
