#include "progress_widget.h"
#include <QPainter>
#include <stdio.h>

ProgressWidget::ProgressWidget(const QString & aName, QWidget * p)
: QWidget(p){
	setFocusPolicy(Qt::NoFocus);
	bgPixmap.load(":images/upgrade_progress_n.png");
	progressPixmap.load(":images/upgrade_progress_s.png");

	progress = 0;

	nameLabel = new QLabel(this);
	QFont f = nameLabel->font();
	f.setPixelSize(24);
	nameLabel->setFont(f);
	nameLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	nameLabel->setIndent(5);
	nameLabel->setGeometry(0, 0, 160, 26);
	setName(aName);

	statusLabel = new QLabel(this);
	statusLabel->setWordWrap(true);
	f = statusLabel->font();
	f.setPixelSize(18);
	statusLabel->setFont(f);
	statusLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	statusLabel->setIndent(5);
	statusLabel->setGeometry(820 + 50, 0, 400, 26);
}

void ProgressWidget::setProgress(int aProgress){
	progress = aProgress;
	update();	
}

void ProgressWidget::setName(const QString & aName){
	if ( aName.contains("-")){
		QString newString;
		QStringList token = aName.split("-");
		for (int i = 0; i < token.count(); ++i){
			if ( i != token.count() - 1)
				newString.append(token.at(i)).append("\n");
			else
				newString.append(token.at(i));
		}
		nameLabel->setText(newString);
	}
	else{
		nameLabel->setText(aName);
	}
}

void ProgressWidget::setCurrentStatus(const QString & aStatus){
	if ( aStatus.contains("-")){
		QString newString;
		QStringList token = aStatus.split("-");
		for (int i = 0; i < token.count(); ++i){
			if ( i != token.count() - 1)
				newString.append(token.at(i)).append("\n");
			else
				newString.append(token.at(i));
		}
		statusLabel->setText(newString);
	}
	else{
		statusLabel->setText(aStatus);
	}
}

void ProgressWidget::paintEvent(QPaintEvent *){
	QPainter painter(this);
	double ratio = progress / 100.0 * 660;
	QPixmap progressCopy  = progressPixmap.copy(0, 0, (int) ratio, progressPixmap.height());
	painter.drawPixmap(160, 0, bgPixmap);
	if ( progress > 0 )
		painter.drawPixmap(160, 0, progressCopy);
	QFont f = painter.font();
	f.setPixelSize(16);
	painter.setFont(f);

	QPen p = painter.pen();
	p.setColor(Qt::white);
	painter.setPen(p);

	painter.drawText(820, 0, 50, 26, Qt::AlignLeft | Qt::AlignVCenter, QString::number(progress) + "%");
}
