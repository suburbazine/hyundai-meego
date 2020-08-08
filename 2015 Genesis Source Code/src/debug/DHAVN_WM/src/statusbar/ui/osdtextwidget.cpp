#include "osdtextwidget.h"
#include <QPainter>
#include "common.h"
#include "dhavnlog.h"
#include "osdview.h"

OSDTextWidget::OSDTextWidget(QWidget * p)
: QWidget(p){
        m_alignment = Qt::AlignLeft | Qt::AlignVCenter;
        m_lineSpacing = -10;
        m_layoutDirection = Qt::LeftToRight;
    m_mode = false;

	QPalette pal = palette();
	pal.setColor(QPalette::WindowText, QColor(250, 250, 250));
	setPalette(pal);
}

void OSDTextWidget::setLineSpacing(int space){
	m_lineSpacing = space;
	update();
}	

void OSDTextWidget::setText(const QString & str){
	m_string = str.split("\n");	
	update();
}

void OSDTextWidget::setAlignment(Qt::Alignment align){
	m_alignment = align;
	update();
}

void OSDTextWidget::setLayoutDirection(Qt::LayoutDirection direction){
        m_layoutDirection = direction;
        update();
}

void OSDTextWidget::setTextColor(bool mode){
    m_mode = mode ;
}
void OSDTextWidget::paintEvent(QPaintEvent *){
	QPainter painter(this);
	QFont f = painter.font();
	        
	if ( m_string.count() == 0)
		return;
	else if ( m_string.count() > 1)
		f.setPointSize(30);
	else
		f.setPointSize(34);

	painter.setFont(f);
	
        painter.setLayoutDirection(m_layoutDirection);

	if ( m_string.count() == 1){
		painter.drawText(0, 0, width(), height(),
			m_alignment | Qt::TextExpandTabs, m_string.at(0));
	}
	else{
		QFontMetrics metrics = painter.fontMetrics();
		int h = metrics.height();
		int contentHeight = (h * m_string.count()) + (m_lineSpacing * (m_string.count() - 1));
		int startY = (height() - contentHeight) / 2;

		if ( startY < 0)
			startY = 0;

		for (int i = 0; i < m_string.count(); ++i){
            lprintf("m_mode : %d", m_mode);
            if(m_mode) {
                if(i == 0) {
                    painter.setPen(OSDView::Grey());
                } else {
                    painter.setPen(OSDView::BrightGrey());
                }
            } else {
                painter.setPen(OSDView::BrightGrey());
            }
            lprintf("m_string.at(%d) : %s", i, qPrintable(m_string.at(i)));
			painter.drawText(0, startY, width(), h, m_alignment | Qt::TextExpandTabs, m_string.at(i));
			startY += (h + m_lineSpacing);
		}	
        m_mode = false;
	}
}
