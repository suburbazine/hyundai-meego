#include "dev_progress_page.h"
#include "progress_widget.h"
#include <stdio.h>
#include <QDebug>
#include <QTouchEvent>
#include <QPainter>

#include <QTimer>

DevProgressPage::DevProgressPage(QWidget * p)
: QWidget(p){
	allUpgradeCount = -1;
	currentIndex = 0;
	rowHeight = -1;
}

void DevProgressPage::setAllUpgradeCount(int count){
	allUpgradeCount = count;

	rowHeight = (720 - 93) / count;
	update();
}

void DevProgressPage::setStatus(const QString & name, const QString & status, int progress){
	for ( int i = 0; i < list.count(); ++i){
		ProgressStatus  * s = list.at(i);
		if ( s->name == name.toUpper()){
			s->status = status;
			s->progress = progress;
			widgetList.at(i)->setProgress(progress);
			widgetList.at(i)->setCurrentStatus(status);
			widgetList.at(i)->update();
			return;
		}
	}

	ProgressStatus * s = new ProgressStatus;
	s->status = status;
	s->progress = progress;
	s->name = name.toUpper();
	list.append(s);

	ProgressWidget * w = new ProgressWidget(name.toUpper(), this);
	w->setProgress(progress);
	w->setCurrentStatus(status);
	widgetList.append(w);
	w->setGeometry(40, 93 + (currentIndex * rowHeight) + (rowHeight - 26) / 2, 
					1240, 26);

	w->show();
	currentIndex++;
}

void DevProgressPage::paintEvent(QPaintEvent *){
	QPainter painter(this);
	QPen p = painter.pen();
	p.setColor(Qt::white);
	painter.setPen(p);

	QFont f = painter.font();
	f.setPixelSize(26);
	painter.setFont(f);

	if ( allUpgradeCount < 0)
		return;
	else{
		for (int i = 0; i < allUpgradeCount; ++i){
			painter.drawText(0, 93 + (i * rowHeight), 50, rowHeight, Qt::AlignRight | Qt::AlignVCenter, QString::number(i + 1));
		}
	}
}


