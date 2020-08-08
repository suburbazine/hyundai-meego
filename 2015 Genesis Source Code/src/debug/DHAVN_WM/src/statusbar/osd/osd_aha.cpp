#include "osd_aha.h"
#include "common.h"
#include "statusicon.h"
#include "osdview.h"
#include "osdtextwidget.h"
#include "dhavnlog.h"


OSDAha::OSDAha(QPixmap * pixmap, int variant)
: AbstractOSDDraw(pixmap, variant){
	command.append("AHA");

	bandLabel = new QLabel(this);
	modeIcon = new QLabel(this);
	modeIcon->setPixmap(QPixmap(g_osd_mode_icons[e_OSD_AHA]));
	infoIcon = new QLabel(this);

        textLabel2Line = new OSDTextWidget(this);
	channelLabel = new QLabel(this);
	dialPalette = channelLabel->palette();	
	normalPalette = dialPalette;
	dialPalette.setColor(QPalette::WindowText, OSDView::DialColor());
	normalPalette.setColor(QPalette::WindowText, OSDView::BrightGrey());

	sepPixmap = new QLabel(this);
	sepPixmap->setPixmap(*pixmap);

	// ignore ME Variant 
	if ( !isMiddleEast()){
                startX = 18;
		modeIcon->setGeometry(startX, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);
		startX += OSD_MODE_ICON_SPACE_WIDTH;

		bandLabel->setGeometry(startX, OSD_ICON_UPPER_MARGIN, 314, OSD_ICON_WH);
		bandLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);	

		sepPixmap->setGeometry(421, 8, 2, 76);

		startX = OSD_SUB_ICON_X;
		infoIcon->setGeometry(startX, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);

		startX += OSD_ICON_SPACE_WIDTH;

		channelLabel->setGeometry(startX, OSD_ICON_UPPER_MARGIN, 1036 - startX, OSD_ICON_WH);
                channelLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
                textLabel2Line->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
                textLabel2Line->setLayoutDirection(Qt::LeftToRight);
	}
}

void OSDAha::setUIPosition(){
	// Ignore ME
}

bool OSDAha::processCommand(const QStringList & args){
	QStringList ahaInfo;
        ahaInfo.append(args);

	if ( getSubCommand() == "WARN")
		infoIcon->setPixmap(QPixmap(g_osd_info_icons[e_INFO_INFO]));
	else
		infoIcon->setPixmap(QPixmap(g_osd_info_icons[e_INFO_STATION]));

        bandLabel->setText(ahaInfo.at(0));

        QString text = g_translate(args.at(1));


        QFont f = font();
        bool containLineFeed = text.contains("\n");
        int startY = (containLineFeed ? 0 : OSD_ICON_UPPER_MARGIN);
        int height = (containLineFeed ? 88 : OSD_ICON_WH);
        if ( containLineFeed ) {

                    setMultiLineGeometry(text);
                    textLabel2Line->setText(text);
                    textLabel2Line->show();
                    channelLabel->hide();
        }
        else{
                f.setPointSize(34);

                channelLabel->setFont(f);
                channelLabel->setGeometry(startX, startY, 1036 - startX, height);

                channelLabel->setText(fitStringToWidget(text, channelLabel));
                textLabel2Line->hide();
                channelLabel->show();
        }
        channelLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

	if ( getSubCommand() == "INFO_DIAL")
		channelLabel->setPalette(dialPalette);
	else
		channelLabel->setPalette(normalPalette);

	return true;
}

void OSDAha::setMultiLineGeometry(QString str) {
    QFont font1 = font();
    lprintf("setMultiLineGeometry >>>> str : %s", qPrintable(str));
    lprintf("setMultiLineGeometry >>>> startX : %d", startX);
    bool multi_containLineFeed = str.contains("\n");
    int startY = (multi_containLineFeed ? 0 : OSD_ICON_UPPER_MARGIN);
    int height = (multi_containLineFeed ? 93 : OSD_ICON_WH);
    if ( multi_containLineFeed )
        font1.setPointSize(30);         // 30
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

int OSDAha::getMinimumArgCount(){
	return 4;
}

const QStringList & OSDAha::getCommand(){
	return command;
}
