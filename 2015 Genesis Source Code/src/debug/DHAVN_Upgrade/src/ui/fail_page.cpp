#include "fail_page.h"
#include "pushbutton.h"

#include <QLabel>
#include <QTextEdit>
#include <QTextStream>
#include <QFile>
#include "common.h"

FailPage::FailPage(QWidget * p)
: QWidget(p){
	remainSecond = 180;
	rebootButtonEnabled = true;
	rebootButton = new PushButton(tr("STR_UPGRADE_RESTART_NOW"), this);
	rebootButton->setGeometry(409, 575, 462, 85);
	
	textEdit = new QTextEdit(this);
	textEdit->setFontPointSize(9);
	textEdit->setGeometry(10, 100, 	1280 - 20, 450);
	textEdit->setFont(QFont("HDB"));

	timer = new QTimer(this);
	timer->setInterval(1000);
	connect(timer, SIGNAL(timeout()), SLOT(decreaseSec()));

	connect(rebootButton, SIGNAL(clicked()), SIGNAL(rebootNow()));
	resize(1280, 720);

	rebootButton->setFocus(true);
}

FailPage::~FailPage(){}

void FailPage::appendFailList(const QString & list){
	QFile f(UPGRADE_LOG_FILE);
	if ( f.open(QIODevice::ReadOnly | QIODevice::Text)){
		QTextStream stream(&f);
		textEdit->append(stream.readAll());
		textEdit->append("\n\n\n");
		f.close();
	}
	textEdit->append(list);
}

void FailPage::jogDialEvent(int e){
	static bool isFocused = true;

	if ( !isVisible())
		return;

	if ( !isFocused){
		isFocused = true;
		rebootButton->setFocus(true);
	}
	else{
		if ( e == 0){
			if ( timer->isActive())
				timer->stop();

			rebootButton->enter();
		}	
	}
}
