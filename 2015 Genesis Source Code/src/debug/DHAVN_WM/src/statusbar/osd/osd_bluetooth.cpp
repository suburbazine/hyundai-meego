#include "osd_bluetooth.h"
#include "infoicon.h"
#include "common.h"
#include "osdview.h"
#include "osdtextwidget.h"
#include "dhavnlog.h"

OSDBluetooth::OSDBluetooth(QPixmap * pixmap, int variant)
: AbstractOSDDraw(pixmap, variant){
	command.append("BLUETOOTH");
	command.append("BLUELINK");

	modeIcon = new QLabel(this);
	//modeIcon->setPixmap(QPixmap(g_osd_mode_icons[e_OSD_BLUETOOTH]));
	

	btLabel = new QLabel(this);
	infoIcon = new InfoIcon(this);

    subLabel = new QLabel(this);
    timeLabel = new QLabel(this);
    textLabel2Line = new OSDTextWidget(this);

	QPalette pal = subLabel->palette();
	pal.setColor(QPalette::WindowText, OSDView::Grey());
	subLabel->setPalette(pal);
	timeLabel->setPalette(pal);


	audioFont = subLabel->font();
	normalFont = audioFont;
	audioFont.setPointSize(28);

	textLabel = new QLabel(this);
        posX = 0;
	
	setUIPosition();
}

void OSDBluetooth::setUIPosition(){
	if ( isMiddleEast()){
		int startX = 1280 - 18 - OSD_ICON_WH;
		modeIcon->setGeometry(startX, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);

		startX -= 8;
		startX -= 314;

		btLabel->setGeometry(startX, OSD_ICON_UPPER_MARGIN, 314, OSD_ICON_WH);
		btLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight | Qt::AlignAbsolute);

		startX -= (15 - 2);

		startX = 1280 - OSD_SUB_ICON_X - 73;
		infoIcon->setGeometry(startX, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);
		posX = startX - 7;

        timeLabel->setGeometry(24, 20, 99, 53);
        subLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight | Qt::AlignAbsolute);
        textLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight | Qt::AlignAbsolute);
        timeLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight | Qt::AlignAbsolute);
        textLabel2Line->setAlignment(Qt::AlignVCenter | Qt::AlignRight | Qt::AlignAbsolute);
        textLabel2Line->setLayoutDirection(Qt::RightToLeft);
    }
    else{
        int startX = 18;
        modeIcon->setGeometry(startX, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);
        startX += OSD_MODE_ICON_SPACE_WIDTH;
        btLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        btLabel->setGeometry(startX, OSD_ICON_UPPER_MARGIN, 314, OSD_ICON_WH);


		startX = OSD_SUB_ICON_X;
		infoIcon->setGeometry(startX, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);
		startX += OSD_ICON_SPACE_WIDTH;
		posX = startX;

        timeLabel->setGeometry(1170, 20, 99, 53);
        //textLabel->setGeometry(startX , OSD_ICON_UPPER_MARGIN, 1280 - startX , OSD_ICON_WH);
        subLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        textLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        timeLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        textLabel2Line->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        textLabel2Line->setLayoutDirection(Qt::LeftToRight);
    }
}

bool OSDBluetooth::processCommand(const QStringList & args){
	timeLabel->hide();

	if ( getTitleCommand() == "BLUETOOTH") 
		modeIcon->setPixmap(QPixmap(g_osd_mode_icons[e_OSD_BLUETOOTH]));
	else if (getTitleCommand() == "BLUELINK")
		modeIcon->setPixmap(QPixmap(g_osd_mode_icons[e_OSD_BLUELINK]));

	if ( getSubCommand() == "AUDIO")
		infoIcon->setIcon(e_INFO_MUSIC);
	else if ( getSubCommand() == "INCOME")
		infoIcon->setIcon(e_INFO_RECEIVE);
	else if ( getSubCommand() == "CALLEND")
		infoIcon->setIcon(e_INFO_ENDCALL);
	else if ( getSubCommand() == "CALLSEND")
		infoIcon->setIcon(e_INFO_OUTCALL);
	else if ( getSubCommand() == "HF")
		infoIcon->setIcon(e_INFO_MAN);
	else if ( getSubCommand() == "CALLING")
		infoIcon->setIcon(e_INFO_CALL);

	btLabel->setText(fitStringToWidget(args.at(0), btLabel));

	if ( (getSubCommand() == "INCOME" || getSubCommand() == "CALLING" || getSubCommand() == "CALLSEND") 
													&& args.count() >= 3){
		subLabel->setFont(normalFont);
		int w = GetStringWidth(textLabel->font(), args.at(1));
		int subW = GetStringWidth(subLabel->font(), args.at(2));
		bool showTimeLabel = (args.count() >= 4);
		bool showSubLabel = false;
		if ( isMiddleEast()){
			int startY = (showTimeLabel ? (1280 - 1155) : (1280 - 1265));
			if ( w + subW + 15 > posX - startY){
				if ( args.at(1).trimmed().isEmpty()){
					subLabel->setGeometry( startY, OSD_ICON_UPPER_MARGIN, (posX - startY), OSD_ICON_WH);
					showSubLabel = true;
					textLabel->setGeometry(0, 0, 0, 0);
				}
				else{
					textLabel->setGeometry( startY, OSD_ICON_UPPER_MARGIN, (posX - startY), OSD_ICON_WH);
				}
			}
			else{
                                //subLabel->setGeometry(startY, OSD_ICON_UPPER_MARGIN, w, OSD_ICON_WH);
                                //textLabel->setGeometry(startY + w + 15, OSD_ICON_UPPER_MARGIN, subW, OSD_ICON_WH);

                               // textLabel->setGeometry(posX, OSD_ICON_UPPER_MARGIN, w, OSD_ICON_WH);
                               // subLabel->setGeometry(posX - subW - 15, OSD_ICON_UPPER_MARGIN, subW, OSD_ICON_WH);

                                textLabel->setGeometry(posX - w -15, OSD_ICON_UPPER_MARGIN, w, OSD_ICON_WH);
                                subLabel->setGeometry(posX - subW - 15 -w, OSD_ICON_UPPER_MARGIN, subW, OSD_ICON_WH);
				showSubLabel = true;
			}
		}
		else{
			int limitY = (showTimeLabel ? 1155 : 1265);
			if ( posX + w + subW + 15 > limitY){	// 15 is gap between text label and sub label
				if ( args.at(1).trimmed().isEmpty()){
					subLabel->setGeometry( posX, OSD_ICON_UPPER_MARGIN, limitY - posX, OSD_ICON_WH);
					showSubLabel = true;
					textLabel->setGeometry(0, 0, 0, 0);
				}
				else{
					textLabel->setGeometry( posX, OSD_ICON_UPPER_MARGIN, limitY - posX, OSD_ICON_WH);
				}
			}
			else{
				textLabel->setGeometry(posX, OSD_ICON_UPPER_MARGIN, w, OSD_ICON_WH);
				subLabel->setGeometry(posX + w + 15, OSD_ICON_UPPER_MARGIN, limitY - posX - w - 15, OSD_ICON_WH);
				showSubLabel = true;
			}
		}

		textLabel->setText(fitStringToWidget(args.at(1), textLabel));	
		subLabel->setText(fitStringToWidget(args.at(2), subLabel));

                    textLabel->show();
                    textLabel2Line->hide();

		if ( showSubLabel)
			subLabel->show();
                else{
                        subLabel->hide();
                }

		if ( showTimeLabel ){
			timeLabel->setText(args.at(3));
			timeLabel->show();
		}
	}
	else if ( getSubCommand() == "AUDIO"){

                QFont f = font();
                bool containLineFeed = args.at(1).contains("\n");
                int startY = (containLineFeed ? 0 : OSD_ICON_UPPER_MARGIN);
                int height = (containLineFeed ? 88 : OSD_ICON_WH);
                if ( containLineFeed ) {
                        f.setPointSize(28);
                }
                else{
                        f.setPointSize(34);
                }

                textLabel->setFont(f);

		if ( isMiddleEast()){
                        textLabel->setGeometry(0, startY+ OSD_ICON_WH / 2, posX, height / 2);
		}
		else{
                        textLabel->setGeometry(posX, startY+ OSD_ICON_WH / 2, 1280 - posX, height / 2);
		}
		subLabel->setFont(audioFont);

		if ( isMiddleEast()){
			subLabel->setGeometry(0, OSD_ICON_UPPER_MARGIN, posX, OSD_ICON_WH / 2);
		}
		else{
			subLabel->setGeometry(posX, OSD_ICON_UPPER_MARGIN, 1280 - posX, OSD_ICON_WH / 2);
		}

		subLabel->setText(fitStringToWidget(args.at(1), subLabel));
		if ( args.count() == 3)
			textLabel->setText(fitStringToWidget(args.at(2), textLabel));	
		else
			textLabel->clear();

        textLabel2Line->hide();
        subLabel->show();
    }
    else{
        subLabel->hide();
//        for(int y = 0 ; y < args.count(); y++) {
//            lprintf("22222 >>>>  args.at(%d) : %s",y, qPrintable(args.at(y)));
//        }

        if(args.at(1) == "MULTI_LINE") {
            QString text = g_translate(args.at(2));
            if (text.contains("\n")) {
                setMultiLineGeometry(text);
                textLabel2Line->setTextColor(true);
                textLabel2Line->setText(text);
                textLabel2Line->show();
                textLabel->hide();
            } else {               
                if ( isMiddleEast()){
                    textLabel->setGeometry(0, OSD_ICON_UPPER_MARGIN, posX, OSD_ICON_WH);
                }
                else{
                    textLabel->setGeometry(posX, OSD_ICON_UPPER_MARGIN, 1280 - posX, OSD_ICON_WH);

                }
                if ( args.count() >= 2){

                    textLabel->setText(fitStringToWidget(text, textLabel));
                }
                textLabel2Line->hide();
                textLabel->show();
            }
        } else {
//            if(textLabel2Line->isVisible()) {
//                textLabel2Line->hide();
//            }

            QFont f = font();
            bool containLineFeed = args.at(1).contains("\n");
            int startY = (containLineFeed ? 0 : OSD_ICON_UPPER_MARGIN);
            int height = (containLineFeed ? 88 : OSD_ICON_WH);
            if ( containLineFeed ) {
                    f.setPointSize(28);
                    setMultiLineGeometry(args.at(1));
                    textLabel2Line->setText(args.at(1));
                    textLabel2Line->show();
                    textLabel->hide();
            }
            else{
                    f.setPointSize(34);

                    textLabel->setFont(f);

                    if ( isMiddleEast()){
                        textLabel->setGeometry(0, startY, posX, height);
                    }
                    else{
                        textLabel->setGeometry(posX, startY, 1280 - posX, height);
                    }

                    if ( args.count() >= 2) {
                        textLabel->setText(fitStringToWidget(args.at(1), textLabel));
                        lprintf("textLabel->text() : %s", qPrintable(textLabel->text()));
                    }
                    textLabel2Line->hide();
                    textLabel->show();
            }

        }
    }

    return true;
}

void OSDBluetooth::setMultiLineGeometry(QString str) {
    QFont font1 = font();
    lprintf("setMultiLineGeometry >>>> str : %s", qPrintable(str));
    lprintf("setMultiLineGeometry >>>> posX : %d", posX);
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
        textLabel2Line->setGeometry(0, startY, posX, height);
    }
    else
    {
        textLabel2Line->setGeometry(posX, startY, 1280 - posX, height);
    }
    return;
}

int OSDBluetooth::getMinimumArgCount(){
	return 4;
}

const QStringList & OSDBluetooth::getCommand(){
	return command;
}

