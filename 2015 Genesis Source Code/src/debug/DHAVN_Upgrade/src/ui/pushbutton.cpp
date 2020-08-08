#include "pushbutton.h"
#include <stdio.h>
#include <QMouseEvent>
#include <QTimer>

PushButton::PushButton(const QString & aStr, QWidget * w): QWidget(w){
	text = aStr;
	pressed = false;
	focused = false;

	QFont f = font();
	f.setPointSize(32);
	setFont(f);

	QPalette pal = palette();
	pal.setColor(QPalette::WindowText, QColor(250, 250, 250));
	setPalette(pal);

	normalPixmap.load(":images/btn_ok_n.png");
	focusPixmap.load(":images/btn_ok_f.png");
	focusPressPixmap.load(":images/btn_ok_fp.png");
	pressPixmap.load(":images/btn_ok_p.png");
}

PushButton::PushButton(QWidget * w, bool loadPixmap)
: QWidget(w){
	text = "";
	pressed = focused = false;

	QFont f = font();
	f.setPointSize(32);
	setFont(f);

	QPalette pal = palette();
	pal.setColor(QPalette::WindowText, QColor(250, 250, 250));
	setPalette(pal);

	if ( loadPixmap ){
		normalPixmap.load(":images/btn_title_back_n.png");
		focusPixmap.load(":images/btn_title_back_f.png");
		focusPressPixmap.load(":images/btn_title_back_f.png");
		pressPixmap.load(":images/btn_title_back_p.png");
	}
}

void PushButton::setNormalPixmap(const QString & str){
	normalPixmap.load(str);
}

void PushButton::setFocusPixmap(const QString & str){
	focusPixmap.load(str);
}

void PushButton::setPressPixmap(const QString & str){
	focusPressPixmap.load(str);
	pressPixmap.load(str);
}

PushButton::~PushButton(){
}

void PushButton::setText(const QString & str){
	text = str;
	update();
}

void PushButton::setFocus(bool f){
	focused = f;
	update();
}

void PushButton::enter(){
	pressed = true;
	update();
	QTimer::singleShot(100, this, SIGNAL(clicked()));
}

void PushButton::release(){
	pressed = false;
	update();
}

void PushButton::mousePressEvent(QMouseEvent *){
	if ( !pressed ){
		pressed = true;
		update();
	}
}

void PushButton::mouseReleaseEvent(QMouseEvent *m){
	if ( pressed ){
		pressed = false;
		update();
		if ( m->x() >= 0 && m->y() >= 0 &&
			m->x() < width() && m->y() << height())
			emit clicked();
	}	
}

void PushButton::paintEvent(QPaintEvent *){
	QPainter painter(this);
	if ( !focused && !pressed )
		painter.drawPixmap(0, 0, normalPixmap);
	else if ( !focused && pressed )
		painter.drawPixmap(0, 0, pressPixmap);
	else if ( focused && !pressed ){
		painter.drawPixmap(0, 0, normalPixmap);
		painter.drawPixmap(0, 0, focusPixmap);
	}
	else
		painter.drawPixmap(0, 0, focusPressPixmap);


	if ( !text.isEmpty()){
		painter.drawText(0, 0, width(), height(),
					Qt::AlignCenter, text);
	}
}
