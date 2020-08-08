#ifndef	OSD_VIEW_H_
#define	OSD_VIEW_H_

#include <QWidget>
#include <QColor>
#include <QList>
#include <QTimer>

#include "abstract_osd_draw.h"

class QStringList;
class QPainter;
class QFontMetrics;
class QStackedWidget;
class OSDData;

class OSDView : public QWidget{
Q_OBJECT
public:
	OSDView(int variant, QWidget * p = 0);

	static QColor BrightGrey(){
		return QColor(250, 250, 250);
	}
	static QColor Grey(){
		return QColor(193, 193, 193);
	}

	static QColor DialColor(){
		return QColor(124, 189, 255);
	}

        static QColor HDChanelColor(){
                return QColor(248,157,28);
        }

	void setRearScreen();
	bool isRearScreen();

	void hideOSD();

private:
	bool m_isShow;

	bool rearScreen;

	QPixmap separator;
	QStackedWidget * stack;

	QList<AbstractOSDDraw *> osdList;

	void startOSD();

public slots:
	void setLanguage(int);

	void displayOSD(OSDData * data);
        void logOSD(const QString &);
};

#endif
