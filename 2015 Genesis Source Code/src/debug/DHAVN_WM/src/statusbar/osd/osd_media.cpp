#include "osd_media.h"
#include "osdview.h"
#include "statusicon.h"
#include "common.h"
#include "infoicon.h"
#include "osdtextwidget.h"
#include "dhavnlog.h"

#include <QDebug>

OSDMedia::OSDMedia(QPixmap * pixmap, int variant)
: AbstractOSDDraw(pixmap, variant){
	command.append("MEDIA");

	timeDisplayType = 0;
	isMusic = false;

	modeIcon = new QLabel(this);
	deviceTypeLabel = new QLabel(this);
        deviceTypeLabel2Line = new OSDTextWidget(this);
      //  deviceTypeLabel2Line = new QMultiLineEdit(this);
	timeLabel = new QLabel(this);
	infoIcon = new InfoIcon(this);
	titleLabel = new QLabel(this);
	titleSubLabel = new QLabel(this);
	subLabel = new QLabel(this);

	repeatIcon = new StatusIcon(this);
	searchIcon = new StatusIcon(this);
	//searchIcon->setIconString("(SE)");
	shuffleIcon = new StatusIcon(this);
	shuffleIcon->setIconString("(SH)");

	numberingLabel = new QLabel(this);

	normalPalette = palette();

	QPalette pal = timeLabel->palette();
	pal.setColor(QPalette::WindowText, OSDView::Grey());
	timeLabel->setPalette(pal);	

	pal = numberingLabel->palette();
	pal.setColor(QPalette::WindowText, OSDView::DialColor());
	numberingLabel->setPalette(pal);
	activePalette = pal;

	pal = subLabel->palette();
	pal.setColor(QPalette::WindowText, OSDView::Grey());
	subLabel->setPalette(pal);


	setUIPosition();
}

void OSDMedia::setUIPosition(){
	if ( isMiddleEast()){
		int startX = 1280 - 18 - OSD_ICON_WH;
		modeIcon->setGeometry(startX, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);
		
		startX -= 8;
		startX -= 314;
                deviceTypeLabelX = startX;
                deviceTypeLabel->setGeometry(startX, OSD_ICON_UPPER_MARGIN, 314, OSD_ICON_WH);
		deviceTypeLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight | Qt::AlignAbsolute);



                deviceTypeLabel2Line->setGeometry(startX, 0, 314, 93);
                deviceTypeLabel2Line->setAlignment(Qt::AlignVCenter | Qt::AlignRight | Qt::AlignAbsolute);
                deviceTypeLabel2Line->setLayoutDirection(Qt::RightToLeft); //dmchoi

		startX -= 15 - 2;

		startX = 1280 - OSD_SUB_ICON_X - 73;
		infoIcon->setGeometry(startX, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);

		startX -= 7;

		titleStartX = startX;
		titleMaxWidth = 380;
		titleLabel->setGeometry(titleStartX - titleMaxWidth, OSD_ICON_UPPER_MARGIN, titleMaxWidth, OSD_ICON_WH);
		titleLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight | Qt::AlignAbsolute);
		titleSubLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight | Qt::AlignAbsolute);

		timeLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight | Qt::AlignAbsolute);
		subLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight | Qt::AlignAbsolute);

		// TODO: numberingLabel geometry
	}
	else{
		int startX = 18;
		modeIcon->setGeometry(startX, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);
		startX += OSD_MODE_ICON_SPACE_WIDTH;
                deviceTypeLabelX = startX;
		deviceTypeLabel->setGeometry(startX, OSD_ICON_UPPER_MARGIN, 314, OSD_ICON_WH);
		deviceTypeLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);


                deviceTypeLabel2Line->setGeometry(startX, 0, 314, 93);
                deviceTypeLabel2Line->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
                deviceTypeLabel2Line->setLayoutDirection(Qt::LeftToRight); //dmchoi

		startX = OSD_SUB_ICON_X;
		infoIcon->setGeometry(startX, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);

		startX += OSD_ICON_SPACE_WIDTH;
		titleStartX = startX;
		titleMaxWidth = 907 - startX;
		titleLabel->setGeometry(titleStartX , OSD_ICON_UPPER_MARGIN, titleMaxWidth , OSD_ICON_WH);
                titleLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft | Qt::AlignAbsolute);
		titleSubLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
		subLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

		timeLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	}		
}

void OSDMedia::setTimeText(const QString & str){
	timeDisplayType = str.count(":");
	qDebug() << "type : " << timeDisplayType;
	if ( timeDisplayType > 0){
		timeLabel->setText(str);
		QFont f;
		f = timeLabel->font();
		if ( timeDisplayType == 2){
			f.setPointSize(32);
		}
		else if ( timeDisplayType == 1){
			f.setPointSize(34);
		}

		timeLabel->setFont(f);

		if ( isMiddleEast()){
			if ( timeDisplayType == 2)
				timeLabel->setGeometry(24, 20, 147, 53);
			else if ( timeDisplayType == 1)
				timeLabel->setGeometry(24, 20, 94 + 5, 53);
		}
		else{
			// Temporarily add width 5, text exceeds gui guideline
			if ( timeDisplayType == 2)
				timeLabel->setGeometry(1120, 20, 147, 53);
			else if ( timeDisplayType == 1)
				timeLabel->setGeometry(1170, 20, 94 + 5, 53);
		}
		timeLabel->show();
	}
	else{
		clearTimeText();
	}
}

void OSDMedia::clearTimeText(){
	timeDisplayType = 0;
	timeLabel->hide();
}


bool OSDMedia::processCommand(const QStringList & args){
	QStringList mediaList;
	mediaList.append(args);
	bool isShuffle = false;
	bool isTime = false;
	//bool isNumbering = false;
	int searchType = 0;
	int repeatType = 0;

	modeIcon->setPixmap(QPixmap(g_osd_mode_icons[ g_string_to_media_index(args.at(0))]));
	deviceTypeLabel->setText(args.at(1));
        QString transText =  g_translate(args.at(1));
        int deviceTypeLabelWidth = AbstractOSDDraw::GetStringWidth(deviceTypeLabel->font(), deviceTypeLabel->text());

     if (deviceTypeLabelWidth>314){
         if(transText.contains("\n")){
             deviceTypeLabel2Line->setText(transText);
             deviceTypeLabel2Line->show();
             deviceTypeLabel->hide();
         }else{
            int index = transText.lastIndexOf(" ");
           
            if (index != -1){
                transText.replace(index,1,"\n");
                deviceTypeLabel2Line->setText(transText);
                deviceTypeLabel2Line->show();
                deviceTypeLabel->hide();
            }else{
                deviceTypeLabel->show();
                deviceTypeLabel2Line->hide();
            }
         }
        }else{
           deviceTypeLabel->show();
           deviceTypeLabel2Line->hide();
        }

        

        if (args.at(2) == "MUSIC"){
		isMusic = true;
		infoIcon->setIcon( e_INFO_MUSIC);
	}
	else if ( args.at(2) == "VIDEO"){
		isMusic = false;
		infoIcon->setIcon( e_INFO_MOVIE);
	}
	else
		infoIcon->setIcon(-1);

	for (int i = 0; i < mediaList.count(); ++i){
		QString arg = mediaList.at(i);
		bool findOk = false;

		if ( arg == "(SE)"){
			findOk = true;
			searchType = SearchAll;
                }else if ( arg == "(FF)"){
                        findOk = true;
                        searchType = SearchFF;
                }else if ( arg == "(REW)"){
                        findOk = true;
                        searchType = SearchREW;
                }
		else if ( arg == "(SF)"){
			findOk = true;
			searchType = SearchFolder;
		}
		else if ( arg == "(SH)"){
			isShuffle = findOk = true;
		}
		else if ( arg == "(RP1)"){
			findOk = true;
			repeatType = RepeatOne;
		}
		else if ( arg == "(RPA)"){
			findOk = true;
			repeatType = RepeatAll;
		}
		else if ( arg == "(RPF)"){
			findOk = true;
			repeatType = RepeatFolder;
		}
		else if ( arg.startsWith("(") && arg.endsWith(")") && arg.contains(":")){
			isTime = findOk = true;
			arg.remove(0, 1);
			arg.remove(arg.length() - 1, 1);
			setTimeText(arg);	
		}
		else if ( arg.startsWith("(") && arg.endsWith(")") && arg.contains("/")){
			// numbering will be deleted, end discuss with UX team(2012-01-03)
			findOk = true;
		}

		if ( findOk){
			mediaList.removeAt(i);
			--i;
		}
	}

	bool existRepeatType = (isShuffle || searchType != 0 || (repeatType != 0));

	if ( mediaList.count() == 5)
		processTitleLabel( mediaList.at(3), getSubCommand(), mediaList.at(4), existRepeatType);
	else if ( mediaList.count() < 4)
		processTitleLabel( "", getSubCommand(), "", existRepeatType);
	else
		processTitleLabel( mediaList.at(3), getSubCommand(), "", existRepeatType);

	if ( !isTime )
		clearTimeText();

	if ( getSubCommand() == "INFO" || getSubCommand() == "INFO_TIME"){
		setMediaStatus(searchType, isShuffle, repeatType);
	}
	else{
		setMediaStatus(0, false, 0);
	}
	
	return true;
}

void OSDMedia::processTitleLabel(const QString & str, const QString & subCommand, const QString & subTitle, bool modeExist){
	int titleWidth;
	int maxWidth = titleMaxWidth;
	//int count;

	if ( !modeExist )
		maxWidth += 197;

	if ( subTitle.isEmpty()){
		titleWidth = maxWidth;
		subLabel->hide();
	}
	else{
		titleWidth = maxWidth / 2;
		// 15 is indent
		if ( isMiddleEast()){
			subLabel->setGeometry( titleStartX - maxWidth, OSD_ICON_UPPER_MARGIN, titleWidth - 15, OSD_ICON_WH);
		}
		else{
			subLabel->setGeometry( titleStartX + titleWidth + 15, OSD_ICON_UPPER_MARGIN, titleWidth - 15, OSD_ICON_WH);
		}
		subLabel->setText( fitStringToWidget( subTitle, subLabel));
		subLabel->show();
	}

	titleSubLabel->hide();
	
	if ( isMiddleEast()){
		titleLabel->setGeometry(titleStartX - titleWidth, OSD_ICON_UPPER_MARGIN, titleWidth, OSD_ICON_WH);
	}
	else{
		titleLabel->setGeometry(titleStartX, OSD_ICON_UPPER_MARGIN, titleWidth, OSD_ICON_WH);
	}
	titleLabel->setText( fitStringToWidget(str, titleLabel));

	if ( subCommand == "INFO" || subCommand == "INFO_TIME")
		titleLabel->setPalette(normalPalette);
	else if ( subCommand == "INFO_DIAL" || subCommand == "INFO_DIAL_TIME")
		titleLabel->setPalette(activePalette);

}

void OSDMedia::setMediaStatus(int searchType, bool isShuffle, int repeatType){
	int displayArray[3] = {0};
	StatusIcon * icons[3];
	int startX = 0;
	bool displayAll = false;
	displayArray[0] = repeatType;
	displayArray[1] = (isShuffle? 1:0);
	displayArray[2] = searchType;
	icons[0] = repeatIcon;
	icons[1] = shuffleIcon;
	icons[2] = searchIcon;
	bool existStatus = (displayArray[0] || displayArray[1] || displayArray[2]);
	
	if ( displayArray[0] > 0 && displayArray[1] > 0 &&
			displayArray[2] > 0 && timeDisplayType > 0)
		displayAll = true;

	if ( isMiddleEast()){           
            startX = 24 + 88 + 65;
	}
        else{
            startX = 1118 - 15;
        }

	for (int i = 0; i < 3; ++i){
		if ( displayArray[i] > 0){
			if ( i == 0 && repeatIcon != NULL){
				switch(displayArray[0]){
				case RepeatAll:
					repeatIcon->setIconString("(RPA)");
					break;
				case RepeatOne:
					repeatIcon->setIconString("(RP1)");
					break;
				case RepeatFolder:
					repeatIcon->setIconString("(RPF)");
					break;
				default:
					break;
				}

				if ( isMiddleEast())
					;
				else
					startX -= 68;

				repeatIcon->setGeometry(startX, 20, 68, 53);
				qDebug() << "show repeatIcon";
			}
			else if ( i == 1){
				if ( isMiddleEast())
					startX += 68;
                                else
                                        startX -= 68;
				shuffleIcon->setGeometry(startX, 20, 53, 53);	
			}
			else {	// i == 2
				switch(displayArray[2]){
				case SearchAll:
					searchIcon->setIconString("(SE)");
					break;
				case SearchFolder:
					searchIcon->setIconString("(SF)");
					break;
                                case SearchFF:
                                        searchIcon->setIconString("(FF)");
                                        break;
                                case SearchREW:
                                        searchIcon->setIconString("(REW)");
                                        break;
				default: break;
				}

				if ( isMiddleEast())
					if ( displayAll)
						startX += 60;
					else
						startX += 68;
				else{
					if ( displayAll)
						startX -= 60;
					else
						startX -= 68;
				}
				searchIcon->setGeometry(startX, 20, 53, 53);
			}
			icons[i]->show();
		}
		else{
			icons[i]->hide();
		}
	}
}

int OSDMedia::getMinimumArgCount(){
	return 6; 
}

const QStringList & OSDMedia::getCommand(){
	return command;
}
