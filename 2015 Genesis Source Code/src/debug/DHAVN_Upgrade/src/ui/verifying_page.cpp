#include "verifying_page.h"
#include "pushbutton.h"
#include "subtitle.h"
#include "common.h"

#include <stdio.h>

VerifyingPage::VerifyingPage(QWidget * w)
: QWidget(w){
	pixmap = new QPixmap[ANI_IMAGE_COUNT];
	const QString prefix = ":images/loading/loading_";
	for (int i = 0; i < ANI_IMAGE_COUNT; ++i){
		QString num = QString::number(i + 1);
		if ( num.length() == 1)
			num.prepend("0");
		pixmap[i].load(prefix + num + ".png");
	}

	pushButton = new PushButton(tr("STR_UPGRADE_CANCEL"), this);
	pushButton->setGeometry(409, 522, 462, 85);
    pushButton->setFocus(false);
    connect(pushButton, SIGNAL(clicked()), SIGNAL(cancel()));


	subTitle = new SubTitle(this);
	subTitle->setGeometry( 0, 93, 1280, 72);

 

	index = 0;
	timer = new QTimer(this);
	timer->setInterval(100);
	connect(timer, SIGNAL(timeout()), SLOT(update()));
//MOBILUS_JWYANG [[ cancel 팝업 2초 후 활성화 되도록 수정
    v_timer = new QTimer(this);
    v_timer->setSingleShot(true);
    connect(v_timer, SIGNAL(timeout()), SLOT(inittimer()));
    v_timer->start(2000);
//MOBILUS_JWYANG ]] cancel 팝업 2초 후 활성화 되도록 수정
}

VerifyingPage::~VerifyingPage(){
    if(timer != NULL)
    {
        if ( timer->isActive())
            timer->stop();
        delete timer;
    }
    if(v_timer != NULL)
    {
        if(v_timer->isActive())
            v_timer->stop();
        delete v_timer;
    }
}
//MOBILUS_JWYANG [[ cancel 팝업 2초 후 활성화 되도록 수정
void VerifyingPage::inittimer(){
    if ( v_timer->isActive())
        v_timer->stop();
    pushButton->setFocus(true);
}
//MOBILUS_JWYANG ]] cancel 팝업 2초 후 활성화 되도록 수정

void VerifyingPage::showEvent(QShowEvent *){
	timer->start();
}

void VerifyingPage::hideEvent(QHideEvent *){
	timer->stop();
}

void VerifyingPage::paintEvent(QPaintEvent * ){
	QPainter painter(this);
	QString validateStr = tr("STR_UPGRADE_VALIDATING");
	QString waitStr = tr("STR_UPGRADE_WAIT");
	painter.drawText(150, 368, 980, 120,
					Qt::AlignCenter, validateStr + "\n" + waitStr);
	painter.drawPixmap(590, 224, pixmap[index]);
	++index;
	if ( index > 15 )
		index = 0;
}

void VerifyingPage::jogDialEvent(int e){
	static bool isFocused = true;

	if ( !isVisible())
		return;

	if ( !isFocused){
		isFocused = true;
		pushButton->setFocus(true);
	}
	else{
		if ( e == 0){
			pushButton->enter();
		} else if (e == 1){
			pushButton->release();
		}
	}
}
