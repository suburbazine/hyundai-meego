#include "airwidget.h"
#include <QLabel>
#include <QPainter>

class StepWidget : public QWidget{
public:
	StepWidget(QWidget * p);
	void setStep(int);

protected:
	void paintEvent(QPaintEvent *);

private:
	int step;
};

StepWidget::StepWidget(QWidget * p)
: QWidget(p){
	QPalette pal = palette();
	pal.setBrush(QPalette::Window, QPixmap(":images/osd/climate/bg_wind_level.png"));
	setPalette(pal);
	setAutoFillBackground(true);
	step = -1;
}

void StepWidget::setStep(int s){
	if ( s >= 0 && s <= 8){
		step = s;
		update();
	}
}

void StepWidget::paintEvent(QPaintEvent *){
	QPainter painter(this);

	if ( step < 1 && step > 8)
		return;

	painter.drawPixmap(0, 0, QPixmap(QString(":images/osd/climate/wind_level_") + QString::number(step) + ".png"));
}


AirWidget::AirWidget(QWidget * p)
: QWidget(p){
	windIcon = new QLabel(this);
	windIcon->setPixmap(QPixmap(":images/osd/climate/ico_wind.png"));
	stepWidget = new StepWidget(this);
	offLabel = new QLabel(this);
	airLabel = new QLabel(this);

	QFont f = offLabel->font();
	f.setPointSize(38);
	offLabel->setFont(f);
	offLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

	QPalette pal = airLabel->palette();
	pal.setColor(QPalette::WindowText, QColor(193, 193, 193));
	airLabel->setPalette(pal);

	f = airLabel->font();
	f.setFamily("HDR");
	f.setPointSize(22);
	airLabel->setFont(f);
	airLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);


	windIcon->setGeometry(0, 5, 53, 56);
	stepWidget->setGeometry(53, 5, 177, 56);

	offLabel->setGeometry(74, 5, 160, 56);
	airLabel->setGeometry(53, 0, 177, 22);
	
	offLabel->setText("OFF");
}

void AirWidget::setAir(int mode){
	if ( mode == 0){
		airLabel->hide();
	}
	else if ( mode == 1){
		airLabel->setText("CLEAN AIR");
		airLabel->show();
	}
	else if ( mode == 2){
		airLabel->setText("ION AIR");
		airLabel->show();
	}
	else if ( mode == 3){
		airLabel->setText("SMART VENT");
		airLabel->show();
	}
	windIcon->show();
}

void AirWidget::setBlow(int value){
	if ( value < 0)
		value = 0;
	else if ( value > 8)
		value = 8;

	windIcon->show();
/*
	if ( value == 0){
		stepWidget->hide();
		airLabel->hide();
		offLabel->show();
	}
	else{*/
		stepWidget->setStep(value);
		stepWidget->show();
		offLabel->hide();
//	}
}
