#include "infoicon.h"
#include "common.h"
#include <QPainter>
#include <QTimer>

// TODO : inherit from StatusIcon or make a parent class 
InfoIcon::InfoIcon(QWidget * p)
: QWidget(p){
	timer = new QTimer(this);
	timer->setInterval(100);
	index = -1;
	animationIndex = -1;
	connect(timer, SIGNAL(timeout()), SLOT(update()));	
}

void InfoIcon::clear(){
	if ( timer->isActive())
		timer->stop();

	index = -1;
	animationIndex = -1;

	update();
}

void InfoIcon::hideEvent(QHideEvent *){
	if ( timer->isActive())
		timer->stop();
}

void InfoIcon::setIcon(int idx){
	if ( idx == -1){
		clear();
		return;
	}
	else if ( idx == index){
		if ( animationIndex != -1){
			animationIndex = 1;
			if ( !timer->isActive())
				timer->start();
		}
		return;
	}

	index = idx;

	if ( index == 0){	// LOADING INFO
		animationIndex = 1;
		timer->start();
	}
	else{
		animationIndex = -1;
	}
	update();
}

void InfoIcon::paintEvent(QPaintEvent *){
	QPainter painter(this);

	if ( index >= 0){
		if ( animationIndex != -1){
			QString num = QString::number(animationIndex);

			if ( num.length() == 1)
				num.prepend("0");

			painter.drawPixmap(0, 0, QString(g_osd_info_icons[index]) + num + ".png");
			animationIndex++;

			if ( animationIndex > 16)
				animationIndex = 1;
		}
		else{
			painter.drawPixmap(0, 0, QString(g_osd_info_icons[index]));
		}
	}
}
