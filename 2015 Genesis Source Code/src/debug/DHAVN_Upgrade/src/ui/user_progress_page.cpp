#include "user_progress_page.h"
#include <QPainter>
#include "common.h"
#include "subtitle.h"
#include <QMouseEvent>


UserProgressPage::UserProgressPage(QWidget * p)
: QWidget(p){
	setFocusPolicy(Qt::NoFocus);
	bgPixmap.load(":images/upgrade_progress_n.png");
	progressPixmap.load(":images/upgrade_progress_s.png");
	progressStatus = NULL;

	m_updateCount = progress = 0;

	m_modeTrigger = false;

	m_weight = 0;

	subTitle = new SubTitle(this);
	subTitle->setGeometry(0, 93, 1280, 72);

	titleLabel = new QLabel(this);
	textLabel1 = new QLabel(this);
	textLabel2 = new QLabel(this);

	QLabel * label[3];
	label[0] = titleLabel;
	label[1] = textLabel1;
	label[2] = textLabel2;

	for (int i = 0; i < 3; ++i){
		QFont f = label[i]->font();

		if ( i == 0)
			f.setPointSize(40);
		else
			f.setPointSize(32);

		label[i]->setFont(f);
		label[i]->setAlignment(Qt::AlignCenter);

		if ( i != 0){
			QPalette pal = label[i]->palette();
			pal.setColor(QPalette::WindowText, Qt::darkGray);
			label[i]->setPalette(pal);
		}
	}

	label[0]->setText(tr("STR_UPGRADE_PREPARED"));
	label[1]->setText(tr("STR_UPGRADE_PROGRESS"));
	label[2]->setText(tr("STR_UPGRADE_NOT_OFF_POWER"));

	label[0]->setGeometry(150, 268 - 20, 980, 50);
	label[1]->setGeometry(150, 268 + 97 - 20, 980, 40);
	label[2]->setGeometry(150, 268 + 97 + 50 - 20, 980, 40);

	resize(1280, 720);
}

UserProgressPage::~UserProgressPage(){
	if ( progressStatus)
		delete progressStatus;
}

void UserProgressPage::setAllUpgradeCount(int count, unsigned int weight){
	m_weight = weight;
	ULOG->log("User All Weight : %u", m_weight);
	m_updateCount = count;
	progressStatus = new ProgressStatus[m_updateCount];
	for ( int i = 0; i < m_updateCount; ++i)
		progressStatus[i].progress = 0;
}

void UserProgressPage::setStatus(const QString & name, const QString & desc, int aProgress, int weight){
	int index = findProgress(name);
	if ( index >= 0){
		progressStatus[index].progress = aProgress;

		if ( name.startsWith("@")){
			titleLabel->setText(tr("STR_UPGRADE_TITLE_CHECK_IMAGE"));
		}
		else{
			titleLabel->setText(tr("STR_UPGRADE_TITLE_SYSTEM_UPGRADE"));
		}
	}
	else{
		bool found = false;
		for (int i = 0; i < m_updateCount; ++i){
			if ( progressStatus[i].name.isEmpty()){
				progressStatus[i].name = name;
				progressStatus[i].progress = aProgress;
				progressStatus[i].weight = weight;
				found = true;
				break;
			}
		}

		if ( !found){
			ULOG_EXCEPTION;
			ULOG->log("Cannot found empty progressStatus SLOT");	
		}
	}

	double currentProgressSum = 0.0;
	for ( int i = 0; i < m_updateCount; ++i){
		currentProgressSum += (double) progressStatus[i].progress * (double) progressStatus[i].weight;	
	}

	double p = currentProgressSum / (double) m_weight * 100.0;
	progress = (int) p;	

	ULOG->log("User Progress Page : %d, %d, %d", (int)currentProgressSum, (int)m_weight, progress);

	update();
}

void UserProgressPage::setStatusDirect(const QString & name, const QString & desc, int aProgress, int weight){
	progress = aProgress;	
	update();
}

int UserProgressPage::findProgress(const QString & name){
	for (int i = 0; i < m_updateCount; ++i)
		if ( progressStatus[i].name == name)
			return i;

	return -1;
}

void UserProgressPage::mousePressEvent(QMouseEvent * m){
	ULOG->log("press : %d, %d", m->x(), m->y());

	if ( m->x() > (310 - 50) && m->x() < (310 + 50) &&
			m->y() > (521 - 50) && m->y() < (521 + 50))
		m_modeTrigger = true;
	else
		m_modeTrigger = false;
}

void UserProgressPage::mouseReleaseEvent(QMouseEvent *m){
	ULOG->log("release : %d, %d", m->x(), m->y());
	// 660, 26
	int endX = 310 + 660;
	if ( m_modeTrigger){
		if ( m->x() > (endX - 50) && m->x() < (endX + 50) &&
			m->y() > (521 - 50) && m->y() < (521 + 50)){
			emit modeChange();
		}
		m_modeTrigger = false;
	}
}



void UserProgressPage::paintEvent(QPaintEvent *){
	QPainter painter(this);

	double ratio = progress / 100.0 * 660;

	QPixmap progressCopy  = progressPixmap.copy(0, 0, (int) ratio, progressPixmap.height());
	painter.drawPixmap(310, 521, bgPixmap);
	if ( progress > 0 )
		painter.drawPixmap(310, 521, progressCopy);
	QFont f = painter.font();
	f.setPointSize(24);
	painter.setFont(f);

	QPen p = painter.pen();
	p.setColor(Qt::darkGray);
	painter.setPen(p);

	painter.drawText(889, 571 - 12, 100, 30, Qt::AlignRight | Qt::AlignVCenter, QString::number(progress) + " %");

}
