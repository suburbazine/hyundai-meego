#ifndef	ABSTRACT_OSD_DRAW_H_
#define ABSTRACT_OSD_DRAW_H_

#include <QWidget>
#include <QObject>
#include <QStringList>
#include <QPainter>
#include <QFont>
#include <QLabel>
#include "statusicon.h"

#define OSD_ICON_SPACE_WIDTH		75
#define OSD_ICON_UPPER_MARGIN		14
#define OSD_ICON_WH					68
#define OSD_MODE_ICON_SPACE_WIDTH	76
#define OSD_MODE_ICON_START_X		18

#define OSD_SUB_ICON_X				428

#define MAX_STATUS_ICON_COUNT		3


class AbstractOSDDraw : public QWidget
{
Q_OBJECT
public:
	AbstractOSDDraw(QPixmap * aSeparator, int variantValue = 0);
	bool isMiddleEast();
	int  getVariant() { return variantValue; }

	bool process(const QStringList & args);
	virtual bool processCommand(const QStringList & args) = 0;
	virtual const QStringList & getCommand() = 0;
	virtual int getMinimumArgCount() = 0;
	virtual void setUIPosition() = 0;

	void	  setRearScreen();
	bool	  isRearScreen();

	QPixmap * getSeperator();

	const QString & getSubCommand() { return subCommand; }
	const QString & getTitleCommand() { return titleCommand; }

	int GetStringWidth(const QFont &, const QString & str);
	int GetStringWidth(QWidget * widget, const QString & str);
	QString fitStringToWidget(const QString &, QWidget * w);
	void setLanguage(int lang);

	void processLastMessage();
	
private:
	int variantValue;
	int languageValue;
	QPixmap * separator;
	QStringList command;
	QString	titleCommand;
	QString subCommand;
	bool rearScreen;
	QStringList lastCommand;

	QString getElidedText(QFontMetrics & metrics, const QString & str, int w);
};

#endif
