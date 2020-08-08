#include "osd_ibox.h"
#include "common.h"
#include "dhavnlog.h"
#include "osdtextwidget.h"
#include "infoicon.h"
#include "statusicon.h"
#include "osdview.h"

typedef struct {
	QString icon_text;
	int icon_index;
} IBoxIcons;

typedef struct {
	QString mnemonic;
	int type;
	int index;
} MediaStatusDef;



const static IBoxIcons mode_to_icon[] = {
	{ "RADIO",		e_OSD_RADIO},
	{ "DMB",		e_OSD_DMB},
	{ "SXM",		e_OSD_SXM},
	{ "DISC",		e_OSD_DISC},
	{ "IPOD",		e_OSD_IPOD},
	{ "AUX",		e_OSD_AUX},
	{ "USB",		e_OSD_USB},
	{ "JUKEBOX",	e_OSD_JUKEBOX},
	{ "BLUETOOTH", 	e_OSD_BLUETOOTH},
	{ "BLUELINK", 	e_OSD_BLUELINK},
	{ "DLNA", 		e_OSD_DLNA},
	{ "APPLICATION", e_OSD_APPLICATION},
	{ "", -1}
};

const static IBoxIcons info_to_icon[] = {
	{ "INFO",			e_INFO_INFO },
	{ "LOADING",		e_INFO_LOADING },
	{ "DISABLE",		e_INFO_DISABLE },
	{ "CALLING",		e_INFO_CALL },
	{ "RECEIVECALL",	e_INFO_RECEIVE },
	{ "OUTCALL",		e_INFO_OUTCALL },
	{ "ENDCALL",		e_INFO_ENDCALL },
	{ "MAN",			e_INFO_MAN },
	{ "BLUELINK",		e_INFO_BLUELINK },
	{ "VOICE",			e_INFO_VOICE },
	{ "MOVIE",			e_INFO_MOVIE },
	{ "MUSIC",			e_INFO_MUSIC },
	{ "CH",				e_INFO_CH },
	{ "SOUND",			e_INFO_SOUND },
	{ "MUTE",			e_INFO_MUTE },
	{ "", -1 }
};

const static MediaStatusDef mnemonic_to_status[] = {
	{ "SCA",	OSDIBox::ScanAll,		1 },
	{ "SCF",	OSDIBox::ScanFolder,	1 },
	{ "SH",		OSDIBox::Shuffle,		2 },
	{ "RPA",	OSDIBox::RepeatAll,		3 },
	{ "RPF",	OSDIBox::RepeatFolder,	3 },
	{ "RP1",	OSDIBox::RepeatOne,		3 },
	{ "",		0, 0}
};


class IBoxMessage {
public:	
	IBoxMessage(){
		clear();
	}

	void clear(){
		m_common = false;
		m_modeIconFile = "";
		m_playTime = "";
		m_playTimeType = m_shuffle = m_repeatType = m_scanType = 0;
	}

	bool common() { return m_common; }
	const QString & modeIconFile() { return m_modeIconFile; }
	const QString & playtime() { return m_playTime; }
	int playTimeType() { return m_playTimeType; }
	int repeatType() { return m_repeatType; }
	int scanType() { return m_scanType; }
	bool shuffle() { return (m_shuffle == 1); }

	void setPlayTimeType(int type) { m_playTimeType = type; }
	void setPlayTime(const QString & t) { m_playTime = t; }
	void setRepeatType(int type) { m_repeatType = type; }
	void setScanType(int type) { m_scanType = type; }
	void setShuffle(int isShuffle) { m_shuffle = isShuffle; }

	void setMode(const QString & mode){
		m_modeIconFile = "";
		if ( mode == "COMMON"){
			m_common = true;
		}
		else{
			m_common = false;
			int index = 0;
			while(1){
				if ( mode_to_icon[index].icon_text.isEmpty())
					break;

				if (mode_to_icon[index].icon_text == mode){
					m_modeIconFile = g_osd_mode_icons[mode_to_icon[index].icon_index];
					return;
				}
				++index;
			}
		}
	}
	bool existPlaytime() { 
		return (!m_playTime.isEmpty());
	}

	bool existStatus() {
		if ( m_repeatType || m_scanType || m_shuffle)
			return true;

		return false;
	}
private:
	bool m_common;
	QString m_modeIconFile;	
	QString m_playTime;
	int m_playTimeType;
	int m_repeatType;
	int m_scanType;
	int m_shuffle;
};


OSDIBox::OSDIBox(QPixmap * pixmap, int variant)
: AbstractOSDDraw(pixmap, variant){
	command.append("IBOX");
	m_message = new IBoxMessage;

	m_modeIcon = new QLabel(this);
	m_infoIcon = new InfoIcon(this);

	m_modeText = new OSDTextWidget(this);
	m_infoText = new OSDTextWidget(this);

	m_repeatIcon = new StatusIcon(this);
	m_scanIcon = new StatusIcon(this);
	m_shuffleIcon = new StatusIcon(this);
	m_shuffleIcon->setIconString("(SH)");
	m_timeLabel = new QLabel(this);

	QPalette pal = m_timeLabel->palette();
	pal.setColor(QPalette::WindowText, OSDView::Grey());
	m_timeLabel->setPalette(pal);

	m_normalPalette = m_infoText->palette();
	m_dialPalette = m_normalPalette;
	m_dialPalette.setColor(QPalette::WindowText, OSDView::DialColor());
}

void OSDIBox::setUIPosition(){
	if ( isMiddleEast()){
		m_timeLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight | Qt::AlignAbsolute);
	}
	else{
		m_timeLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	}
}

const QStringList & OSDIBox::getCommand(){
	return command;
}

void OSDIBox::hideWidgets(){
	m_modeIcon->hide();
	m_infoIcon->hide();
	m_modeText->hide();
	m_infoText->hide();	
	m_repeatIcon->hide();
	m_scanIcon->hide();
	m_shuffleIcon->hide();
	m_timeLabel->clear();
	m_timeLabel->hide();
}

QString OSDIBox::fetchIconFromText(const QString & str, int & index){
	index = -1;
	int closeIndex = 0;
	if ( str.startsWith("(")){
		if ( (closeIndex = str.indexOf(QChar(')'))) > -1){
			if ( closeIndex == 1 ) return QString(str);	//empty icon string

			QString iconString = str.mid(1, closeIndex - 1);
			int iconIndex = infoStringToIndex(iconString);

			if ( iconIndex >= 0 ){ 
				index = iconIndex;
				return str.mid(closeIndex + 1, str.length() - (closeIndex + 1));
			}
		}
	}
	return QString(str);
}

void OSDIBox::processCommonOSD(const QString & str){
	int iconIndex = -1;
	QString infoText = fetchIconFromText(str, iconIndex);

	bool containLineFeed = infoText.contains("\n");
	int startY = (containLineFeed ? 0 : OSD_ICON_UPPER_MARGIN);
	int height = (containLineFeed ? 93 : OSD_ICON_WH);

	QFont f = font();
	if ( containLineFeed )
		f.setPointSize(30);
	else
		f.setPointSize(34);

	int textWidth = AbstractOSDDraw::GetStringWidth(f, infoText);
	int iconTextWidth = 0;
	int startX = 0;
	if ( iconIndex == -1){
		if ( textWidth > 1280 ) textWidth = 1280;
		iconTextWidth = textWidth;
		startX = (1280 - textWidth) / 2;
	}
	else{
		if ( textWidth > 1280 - OSD_ICON_SPACE_WIDTH)
			textWidth = 1280 - OSD_ICON_SPACE_WIDTH;

		iconTextWidth = textWidth + OSD_ICON_SPACE_WIDTH;

		startX = (1280 - iconTextWidth ) / 2;
		m_infoIcon->setIcon(iconIndex);
	}


	if ( isMiddleEast()){
                m_infoText->setAlignment(Qt::AlignRight | Qt::AlignVCenter | Qt::AlignAbsolute);
                m_infoText->setLayoutDirection(Qt::RightToLeft); //dmchoi
		if ( iconIndex == -1){
			m_infoText->setGeometry(startX, startY, textWidth, height);
		}
		else{
			startX = 1280 - startX;
			m_infoIcon->setGeometry(startX - OSD_ICON_SPACE_WIDTH + 8, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);
			m_infoText->setGeometry(startX - iconTextWidth, startY, textWidth, height);
		}
	}
	else{
		m_infoText->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
                m_infoText->setLayoutDirection(Qt::LeftToRight); //dmchoi
		if ( iconIndex == -1){
			m_infoText->setGeometry(startX, startY, textWidth, height);
		}
		else{
			m_infoIcon->setGeometry(startX, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);
			m_infoText->setGeometry(startX + OSD_ICON_SPACE_WIDTH , startY, textWidth, height);
		}
	}

	m_infoText->setFont(f);
	m_infoText->setText(fitStringToWidget(infoText, m_infoText));

	if ( iconIndex == -1){
		m_infoText->show();
	}
	else{
		m_infoText->show();
		m_infoIcon->show();
	}
}

void OSDIBox::processModeOSD(const QString & modeStr, const QString & infoStr){
	int iconIndex = -1;
	QString infoText = fetchIconFromText(infoStr, iconIndex);

	bool modeTextContainLineFeed = modeStr.contains("\n");
	int modeStartY = (modeTextContainLineFeed ? 0 : OSD_ICON_UPPER_MARGIN);
	int modeHeight = (modeTextContainLineFeed ? 93 : OSD_ICON_WH);

	if ( isMiddleEast()){
		m_modeText->setAlignment(Qt::AlignVCenter | Qt::AlignRight | Qt::AlignAbsolute);
                m_modeText->setLayoutDirection(Qt::RightToLeft); //dmchoi
		m_modeText->setGeometry(1280 - OSD_MODE_ICON_SPACE_WIDTH - 314, modeStartY, 314, modeHeight);
	}
	else{
		m_modeText->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
                m_modeText->setLayoutDirection(Qt::LeftToRight); //dmchoi
		m_modeText->setGeometry(OSD_MODE_ICON_SPACE_WIDTH + OSD_MODE_ICON_START_X, modeStartY, 314, modeHeight);
	}

	QFont f = font();
	if ( modeTextContainLineFeed ) f.setPointSize(30);
	else f.setPointSize(34);
	m_modeText->setFont(f);

	m_modeText->setText(fitStringToWidget(modeStr, m_modeText));

	bool infoTextContainLineFeed = infoStr.contains("\n");
	int infoStartY = (infoTextContainLineFeed ? 0 : OSD_ICON_UPPER_MARGIN);
	int infoHeight = (infoTextContainLineFeed ? 93 : OSD_ICON_WH);

	if ( infoTextContainLineFeed) f.setPointSize(30);
	else f.setPointSize(34);
	m_infoText->setFont(f);

	m_infoIcon->setIcon(iconIndex);

	int infoTextWidth = 380;
	if ( ! m_message->existStatus() && !m_message->existPlaytime()) infoTextWidth += 197 + 147;
	else if ( ! m_message->existStatus()) infoTextWidth += 197;
	
	if ( isMiddleEast()){
		m_infoIcon->setGeometry(1280 - OSD_SUB_ICON_X - 73, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);
		m_infoText->setAlignment(Qt::AlignVCenter | Qt::AlignRight | Qt::AlignAbsolute);
		m_infoText->setGeometry(1280 - OSD_SUB_ICON_X - 80 - infoTextWidth, infoStartY, infoTextWidth, infoHeight);
	}
	else{
		m_infoIcon->setGeometry(OSD_SUB_ICON_X, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);
		m_infoText->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
		m_infoText->setGeometry(OSD_SUB_ICON_X + OSD_ICON_SPACE_WIDTH, infoStartY, infoTextWidth, infoHeight);
	}

	m_modeText->show();
	m_infoIcon->show();
	m_infoText->show();

	m_infoText->setText(fitStringToWidget(infoText, m_infoText));

}

void OSDIBox::processModeIcon(){
	m_modeIcon->setPixmap(m_message->modeIconFile());
	if ( isMiddleEast())
		m_modeIcon->setGeometry(1280 - OSD_MODE_ICON_START_X - OSD_ICON_WH, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);
	else
		m_modeIcon->setGeometry(OSD_MODE_ICON_START_X, OSD_ICON_UPPER_MARGIN, OSD_ICON_WH, OSD_ICON_WH);

	m_modeIcon->show();
}

void OSDIBox::findOccurence(QList<int>& saveList, const QString & origin, const QString & str){
	int from = 0;
	if ( origin.isNull() || origin.isEmpty()) return;

	while(1){
		from = origin.indexOf(str, from);
		if ( from == -1) return;

		saveList.append(from);
		from++;	
	}
}

void OSDIBox::parsingStatusOSD(const QString & status){
	QStringList tokens;

	QList<int> openBrackets;
	QList<int> closeBrackets;

	findOccurence(openBrackets, status, "(");
	findOccurence(closeBrackets, status, ")");

	if ( openBrackets.count() != 0 && openBrackets.count() == closeBrackets.count()){
		for (int i = 0; i < openBrackets.count(); ++i){
			if ( openBrackets.at(i) + 1 == closeBrackets.at(i))
				continue;

			setStatus( status.mid(openBrackets.at(i) + 1,
						closeBrackets.at(i) - openBrackets.at(i) - 1));
		}
	}
}

void OSDIBox::setStatus(const QString & str){
	if ( str.contains(":")){
		QStringList tokens = str.split(":");
		if ( tokens.count() == 2 || tokens.count() == 3){
			bool isOk = false;
			for (int i = 0; i < tokens.count(); ++i){
				tokens.at(i).toInt(&isOk);
				if ( !isOk ) return;
			}
			if ( tokens.count() == 2 ) m_message->setPlayTimeType(1);
			else m_message->setPlayTimeType(2);

			m_message->setPlayTime(str);
		}
	}
	else{
		int index = 0;
		while(1){
			if ( mnemonic_to_status[index].mnemonic.isEmpty()) break;

			if ( mnemonic_to_status[index].mnemonic == str){
				switch(mnemonic_to_status[index].index){
				case 1:	
					m_message->setScanType(mnemonic_to_status[index].type);
					break;
				case 2:
					m_message->setShuffle(1);
					break;
				case 3:
					m_message->setRepeatType(mnemonic_to_status[index].type);
					break;
				default: break;
				}
			}

			++index;
		}
	}
}

void OSDIBox::applyTextColor(const QString & color){
	if ( color == "TUNE")
		m_infoText->setPalette(m_dialPalette);
	else
		m_infoText->setPalette(m_normalPalette);
}

bool OSDIBox::processCommand(const QStringList & args){
	hideWidgets();
	m_message->clear();

	m_message->setMode(getSubCommand());

	if ( args.count() > 1){
		applyTextColor(args.at(0));
		if ( m_message->common()){
			processCommonOSD(args.at(1));	
		}
		else{
			processModeIcon();
			if ( args.count() > 3)
				parsingStatusOSD(args.at(3));

			if ( args.count() >= 3){
				processModeOSD(args.at(1), args.at(2));
				if ( m_message->existPlaytime())
					setTimeText(m_message->playtime());

				if ( m_message->existStatus())
					setMediaStatus(m_message->scanType(), m_message->shuffle(), m_message->repeatType());
			}
		}
		return true;
	}
	return false;
}

void OSDIBox::setMediaStatus(int scanType, bool isShuffle, int repeatType){
	int displayArray[3] = {0};
	StatusIcon * icons[3];
	int startX = 0;
	bool displayAll = false;
	displayArray[0] = repeatType;
	displayArray[1] = (isShuffle? 1:0);
	displayArray[2] = scanType;
	icons[0] = m_repeatIcon;
	icons[1] = m_shuffleIcon;
	icons[2] = m_scanIcon;

	int timeDisplayType = m_message->playTimeType();
	
	if ( displayArray[0] > 0 && displayArray[1] > 0 &&
			displayArray[2] > 0 && timeDisplayType > 0)
		displayAll = true;


	if ( isMiddleEast()){
		if ( timeDisplayType == 1)
			startX = 24 + 138 + 15;
		else if ( timeDisplayType == 2)
			startX = 24 + 88 + 65;
		else
			startX = 24;
	}
	else{
		if ( timeDisplayType == 1)
			startX = 1168 - 15;	
		else if ( timeDisplayType == 2)
			startX = 1118 - 15;
		else
			startX = 1267;               
	}

	for (int i = 0; i < 3; ++i){
		if ( displayArray[i] > 0){
			if ( i == 0 && m_repeatIcon != NULL){
				switch(displayArray[0]){
				case RepeatAll:
					m_repeatIcon->setIconString("(RPA)");
					break;
				case RepeatOne:
					m_repeatIcon->setIconString("(RP1)");
					break;
				case RepeatFolder:
					m_repeatIcon->setIconString("(RPF)");
					break;
				default:
					break;
				}

				if ( isMiddleEast())
					;
				else
					startX -= 68;

				m_repeatIcon->setGeometry(startX, 20, 68, 53);
			}
			else if ( i == 1){
				if ( isMiddleEast())
					startX += 68;
				else
					startX -= 68;
				m_shuffleIcon->setGeometry(startX, 20, 53, 53);	
			}
			else {	// i == 2
				switch(displayArray[2]){
				case ScanAll:
					m_scanIcon->setIconString("(SE)");
					break;
				case ScanFolder:
					m_scanIcon->setIconString("(SF)");
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
				m_scanIcon->setGeometry(startX, 20, 53, 53);
			}
			icons[i]->show();
		}
		else{
			icons[i]->hide();
		}
	}

}

void OSDIBox::setTimeText(const QString & str){
	QFont f = m_timeLabel->font();
	if ( m_message->playTimeType() == 1) f.setPointSize(34);
	else if ( m_message->playTimeType() == 2) f.setPointSize(32);
	m_timeLabel->setFont(f);

	m_timeLabel->setText(str);

	if ( isMiddleEast()){
		if ( m_message->playTimeType() == 1)
			m_timeLabel->setGeometry(24, 20, 94 + 5, 53);
		else if ( m_message->playTimeType() == 2)
			m_timeLabel->setGeometry(24, 20, 147, 53);
	}
	else{
		if ( m_message->playTimeType() == 1)
			m_timeLabel->setGeometry(1170, 20, 94 + 5, 53);
		else if ( m_message->playTimeType() == 2)
			m_timeLabel->setGeometry(1120, 20, 147, 53);
	}
	m_timeLabel->show();
}

int OSDIBox::infoStringToIndex(const QString & str){
	int index = 0;
	while(1){
		if ( info_to_icon[index].icon_text.isEmpty())
			break;

		if ( info_to_icon[index].icon_text == str){
			return info_to_icon[index].icon_index;
		}
		
		++index;
	}
	return -1;
}

int OSDIBox::getMinimumArgCount(){
	return 4;
}



