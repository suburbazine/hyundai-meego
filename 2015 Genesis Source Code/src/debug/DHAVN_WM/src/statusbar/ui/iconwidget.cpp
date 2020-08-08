#include "iconwidget.h"
#include <QPainter>
#include "common.h"
#include "dhavnlog.h"


IconWidget::IconWidget(QWidget * p) : QWidget(p)
{
	timer = new QTimer(this);
	timer->setInterval(500);
	connect(timer, SIGNAL(timeout()), SLOT(update()));	

	m_index = UNKNOWN_VALUE;
	m_animationIndex = UNKNOWN_VALUE;
	m_value = -1;
}

void IconWidget::setIndex(int i){
	if ( i == UNKNOWN_VALUE){
		if ( timer->isActive())
			timer->stop();
		m_animationIndex = UNKNOWN_VALUE;
		m_index = UNKNOWN_VALUE;
	}
	else if(i > UNKNOWN_VALUE){
		if ( m_index == i && m_index > UNKNOWN_VALUE && m_value == g_status_table[m_index].value)
			return;

		m_index = i;

		m_value = g_status_table[m_index].value;

		if ( g_status_table[m_index].icon_count > 1 && g_status_table[m_index].value == -1 ){
			m_animationIndex = g_status_table[m_index].icon_start_index;
			if ( !timer->isActive())
				timer->start();
		}
		else{
			m_animationIndex = UNKNOWN_VALUE;
			if ( timer->isActive())
				timer->stop();
		}
	}
	update();
}

void IconWidget::paintEvent(QPaintEvent *){
	QPainter painter(this);

	if ( m_index == UNKNOWN_VALUE )
		return;

	if ( m_animationIndex != UNKNOWN_VALUE){
		painter.drawPixmap(0, 0, QPixmap( 
			QString(g_status_table[m_index].icon_name_prefix).append(QString::number(m_animationIndex) + ".png")));

		if ( m_animationIndex >= g_status_table[m_index].icon_count + g_status_table[m_index].icon_start_index - 1){
			m_animationIndex = g_status_table[m_index].icon_start_index;
		}
		else{
			m_animationIndex++;
		}
	}
	else{
		QString pix(g_status_table[m_index].icon_name_prefix);
		if ( g_status_table[m_index].value != -1){
			pix.append( QString::number(g_status_table[m_index].value));
		}
		painter.drawPixmap(0, 0, QPixmap( pix.append(".png")));
	}
}
