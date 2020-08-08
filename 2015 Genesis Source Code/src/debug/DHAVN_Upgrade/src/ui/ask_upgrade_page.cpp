#include "ask_upgrade_page.h"
#include "pushbutton.h"
#include "subtitle.h"

AskUpgradePage::AskUpgradePage(QWidget * p)
: QWidget(p){
	okButton = new PushButton(tr("STR_UPGRADE_YES"), this);
	cancelButton = new PushButton(tr("STR_UPGRADE_NO"), this);
	subTitle = new SubTitle(this);

	QFont f = okButton->font();
	f.setPointSize(36);
	okButton->setFont(f);
	cancelButton->setFont(f);

	subTitle->setGeometry( 0, 93, 1280, 72);

	okButton->setGeometry( 170, 477, 462, 85);
	cancelButton->setGeometry( 170 + 478, 477, 462, 85);

	QString latestStr = tr("STR_UPGRADE_LATEST");
	QString askStr = tr("STR_UPGRADE_START");
	label = new QLabel(latestStr + "\n" + askStr, this);
	label->setGeometry(150, 313, 960, 120);
	label->setAlignment(Qt::AlignCenter);
	connect(okButton, SIGNAL(clicked()), SIGNAL(upgradeConfirm()));
	connect(cancelButton, SIGNAL(clicked()), SIGNAL(upgradeCancel()));
	connect(subTitle, SIGNAL(back()), SIGNAL(upgradeCancel()));
}

AskUpgradePage::~AskUpgradePage(){}

void AskUpgradePage::jogDialEvent(int e){
	static bool isFocused = false;

	if ( !isVisible())
		return;

	if ( !isFocused){
		isFocused = true;
		okButton->setFocus(true);
	}
	else{
		if ( e == 0){
			if ( okButton->isFocused())
				okButton->enter();
			else
				cancelButton->enter();
		}
		else{
			if ( okButton->isFocused()){
				okButton->setFocus(false);
				cancelButton->setFocus(true);	
			}
			else{
				okButton->setFocus(true);
				cancelButton->setFocus(false);
			}
		}	
	}

}
