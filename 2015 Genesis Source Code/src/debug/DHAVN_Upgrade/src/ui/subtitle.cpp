#include "subtitle.h"

SubTitle::SubTitle(QWidget * p)
: QWidget(p){
	QPalette pal = palette();
	pal.setBrush(QPalette::Window, QPixmap(":images/bg_title.png"));
	setPalette(pal);
	setAutoFillBackground(true);

	m_titleLabel = new QLabel(this);
	m_titleLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        m_titleLabel->setGeometry(46, 0, 1200, 72);

	QFont f = m_titleLabel->font();
	f.setPointSize(40);
	m_titleLabel->setFont(f);

	pal = m_titleLabel->palette();
	pal.setColor(QPalette::WindowText, QColor(250, 250, 250));
	m_titleLabel->setPalette(pal);

	m_titleLabel->setText(tr("STR_UPGRADE"));

}
