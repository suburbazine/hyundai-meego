#ifndef	AIR_WIDGET_H_
#define	AIR_WIDGET_H_

#include <QWidget>

class QLabel;
class StepWidget;

class AirWidget : public QWidget{
public:
	AirWidget(QWidget * p);

	void setAir(int mode);
	void setBlow(int value);

private:
	QLabel * windIcon;
	QLabel * offLabel;
	QLabel * airLabel;
	StepWidget * stepWidget;
};

#endif
