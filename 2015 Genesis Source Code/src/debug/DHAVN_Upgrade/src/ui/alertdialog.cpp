#include "alertdialog.h"
#include "pushbutton.h"

AlertDialog::AlertDialog(QWidget * p)
: QWidget(p){
	QPalette pal = palette();
	pal.setColor(QPalette::Window, QColor(0, 0, 0, 255 - 77));
	pal.setColor(QPalette::WindowText, QColor(250, 250, 250));
	setPalette(pal);

	setAutoFillBackground(true);

	QFont f = font();
	f.setPointSize(36);
	setFont(f);

	label = new QLabel(this);
	pal = label->palette();
	pal.setBrush(QPalette::Window, QPixmap(":images/popup/bg_type_a.png"));
	label->setPalette(pal);
	label->setAutoFillBackground(true);
	label->setAlignment(Qt::AlignLeft| Qt::AlignVCenter);
	label->setIndent(70);
	label->setText(tr("STR_UPGRADE_USB_DETACHED"));
	label->setGeometry(93, 208, 1093, 304);

	button = new PushButton(this, false);
	button->setNormalPixmap(":images/popup/btn_type_a_01_n.png");			
	button->setFocusPixmap(":images/popup/btn_type_a_01_f.png");			
	button->setPressPixmap(":images/popup/btn_type_a_01_p.png");
	button->setGeometry(93 + 780, 208 + 25, 288, 254);
	button->setFocus(true);
	button->setText(tr("STR_UPGRADE_RESTART_NOW"));

	connect(button, SIGNAL(clicked()), SIGNAL(exitConfirmed()));
}

void AlertDialog::jogDialEvent(int e){
	if ( !isVisible())
		return;

	if ( e == 0){
		button->enter();		
	}
}
