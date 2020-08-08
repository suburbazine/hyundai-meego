#include "abstract_osd_draw.h"
#include <QFontMetrics>
#include "statusicon.h"

#include "dhavnlog.h"

AbstractOSDDraw::AbstractOSDDraw(QPixmap * aSeparator, int aVariant)
:QWidget(), variantValue(aVariant), separator(aSeparator) 
{
	languageValue = 0;
	rearScreen = false;
	QPalette pal = palette();
	pal.setBrush(QPalette::Window, QPixmap(":images/bg_indi.png"));
	setPalette(pal);
	setAutoFillBackground(true);
}

QPixmap * AbstractOSDDraw::getSeperator(){
	return separator;
}

int AbstractOSDDraw::GetStringWidth(const QFont & f, const QString & str){
	QFontMetrics metrics(f);
	QRect r = metrics.boundingRect(0, 0, 1280, 93, Qt::TextDontClip | Qt::TextExpandTabs, str);

	return r.width();
}

int AbstractOSDDraw::GetStringWidth(QWidget * widget, const QString & str){
	QFontMetrics metrics = widget->fontMetrics();
	QRect r = metrics.boundingRect(0, 0, 1280, 93, Qt::TextDontClip | Qt::TextExpandTabs, str);

	return r.width();
}

void AbstractOSDDraw::setRearScreen(){
	rearScreen = true;
}

bool AbstractOSDDraw::isRearScreen(){
	return rearScreen;
}	

void AbstractOSDDraw::setLanguage(int lang){
	languageValue = lang;
	setUIPosition();
}

void AbstractOSDDraw::processLastMessage(){
	if ( !lastCommand.isEmpty())
		process(lastCommand);
}

bool AbstractOSDDraw::process(const QStringList & args){
	lastCommand = args;
	if ( args.count() >= getMinimumArgCount()){
		QStringList commandArg;
		commandArg.append(args);
		titleCommand = commandArg.at(0);
		commandArg.removeFirst();
		subCommand = commandArg.takeFirst();
		bool processed =  processCommand(commandArg);
	//	update();
		return processed;
	}
	return false;
}

bool AbstractOSDDraw::isMiddleEast(){
	return (variantValue == 4 && languageValue == 8);
}

QString AbstractOSDDraw::fitStringToWidget(const QString & str, QWidget * w){
	QFontMetrics metrics = w->fontMetrics();
	QString fitString;
	
	QStringList lineToken = str.split("\n");
	QStringList formattedList;

	for (int i = 0; i < lineToken.count(); ++i)
		formattedList.append( getElidedText(metrics, lineToken.at(i), w->width()));
		
	for (int i = 0; i < formattedList.count(); ++i){
		if ( i != 0)
			fitString.append("\n");

		fitString.append(formattedList.at(i));
	}

	return fitString;
}

QString AbstractOSDDraw::getElidedText(QFontMetrics & metrics, const QString & str, int w){
	QString resultLine = str;
	QString shortStr;
	shortStr.append(QChar(0x00b7)).append(QChar(0x00b7)).append(QChar(0x00b7));
	int avg = metrics.averageCharWidth();
	int textWidth = metrics.boundingRect(0, 0, 1280, 720, Qt::TextDontClip | Qt::TextExpandTabs, str).width();

	if ( textWidth <= w)
		return resultLine;	

	// Max count == 20
	for (int i = 0; i < 20; ++i){
		int exceedWidth = textWidth - w;
		int removeCount = (exceedWidth / avg) + 1;
		resultLine.remove( resultLine.length() - removeCount, removeCount);

		if ( metrics.boundingRect(0, 0, 1280, 720, Qt::TextDontClip | Qt::TextExpandTabs, resultLine + shortStr).width() <= w){
			break;
		}
		else{
			textWidth = metrics.boundingRect(0, 0, 1280, 720, Qt::TextDontClip | Qt::TextExpandTabs, resultLine).width();
		}
	}

	return resultLine + shortStr;	
}

