#include "no_upgrade_page.h"
#include "pushbutton.h"
#include "subtitle.h"

NoUpgradePage::NoUpgradePage(QWidget * p)
: QWidget(p){
	okButton = new PushButton(tr("STR_UPGRADE_RESTART_NOW"), this);
	okButton->setGeometry(150 + 259, 477, 462, 85);

	connect(okButton, SIGNAL(clicked()),
				SIGNAL(cancelConfirm()));

	subTitle = new SubTitle(this);
	subTitle->setGeometry(0, 93, 1280, 72);

	label = new QLabel(tr("STR_UPGRADE_NO_AVAILABLE"), this);
        //label->setGeometry(150, 333 - 20, 980, 120);
        label->setGeometry(0, 333 - 20, 1280, 120);
	label->setAlignment(Qt::AlignCenter);

	okButton->setFocus(true);
}

NoUpgradePage::~NoUpgradePage(){}

void NoUpgradePage::setText(const QString & t){
	label->setText(t);
}

void NoUpgradePage::jogDialEvent(int e){
	static bool isFocused = true;

	if ( !isVisible())
		return;

	if ( !isFocused){
		isFocused = true;
		okButton->setFocus(true);
	}
	else{
		if ( e == 0){
			okButton->enter();
		}	
	}
}
