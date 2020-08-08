#include "statusicon.h"
#include "common.h"
#include <QPainter>
#include <QTimer>
#include <QDebug>

StatusIcon::StatusIcon(QWidget * p)
: QWidget(p){
	timer = new QTimer(this);
	timer->setInterval(100);
	index = -1;
	animationIndex = -1;
	connect(timer, SIGNAL(timeout()), SLOT(update()));	
}

int StatusIcon::IconIndex(const QString & str){
	int index = 0;
	while(1){
		if ( g_osd_status_icons[index].string == NULL)
			break;

		if ( g_osd_status_icons[index].string == str)
			return index;
		index++;
	}
	return -1;
}

void StatusIcon::clear(){
	if ( timer->isActive())
		timer->stop();

	index = -1;
	animationIndex = -1;
	iconString.clear();
}

void StatusIcon::hideEvent(QHideEvent *){
	qDebug() << "hideEvent";
	if ( timer->isActive())
		timer->stop();
}

void StatusIcon::setIconString(const QString & str){
	if ( iconString == str){
		if ( animationIndex != -1){
			animationIndex = g_osd_status_icons[index].start_ani_index;
			if ( !timer->isActive())
				timer->start();
		}
		return;
	}

	iconString = str;
	index = IconIndex(iconString);

	if ( index >= 0 && g_osd_status_icons[index].ani_count > 0){
		animationIndex = g_osd_status_icons[index].start_ani_index;
		timer->start();
	}
	else{
		animationIndex = -1;
	}
	update();
}

void StatusIcon::paintEvent(QPaintEvent *){
	QPainter painter(this);

	if ( index >= 0){
		if ( animationIndex != -1){
			QString num = QString::number(animationIndex);

			if ( num.length() == 1)
				num.prepend("0");

			painter.drawPixmap(0, 0, QString(g_osd_status_icons[index].icon_name) + num + ".png");
			animationIndex++;

			if ( animationIndex > g_osd_status_icons[index].ani_count + g_osd_status_icons[index].start_ani_index - 1)
				animationIndex = g_osd_status_icons[index].start_ani_index;
		}
		else{
			painter.drawPixmap(0, 0, QString(g_osd_status_icons[index].icon_name));
		}
	}
}
