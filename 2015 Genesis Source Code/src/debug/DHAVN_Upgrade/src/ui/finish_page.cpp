#include "finish_page.h"

#include <QLabel>
#include <QTimer>
#include "common.h"
FinishPage::FinishPage(QWidget * p)
: QWidget(p)
{
	remainSecond = 10;

	completeStr = tr("STR_UPGRADE_COMPLETE");
	rebootStr = tr("STR_UPGRADE_RESTART");
	


        if ( g_languageId != 1 ) // LANG_ko_KR
        {

            secStr = tr("STR_UPGRADE_SECOND");

            QString secStrD = QString(secStr).arg(remainSecond);


            label = new QLabel(completeStr + "\n"  + secStrD, this);

        }else{
            secStr = tr("STR_UPGRADE_SECOND");
            label = new QLabel(completeStr + "\n" /*+ rebootStr + " "*/ +
                                                    QString::number(remainSecond) + " " + secStr, this);
        }

        //label->setGeometry(150, 261, 980, 120);
        label->setGeometry(0, 261, 1280, 120);
	label->setAlignment(Qt::AlignCenter);

	timer = new QTimer(this);
	timer->setInterval(1000);
	connect(timer, SIGNAL(timeout()), SLOT(decreaseSec()));
}

FinishPage::~FinishPage(){}

void FinishPage::showEvent(QShowEvent *){
	if ( !timer->isActive()) timer->start();
}

void FinishPage::rebootClicked(){
	if ( timer->isActive())
		timer->stop();

	emit rebootNow();
}

void FinishPage::decreaseSec(){
	remainSecond--;


        if ( g_languageId != 1 ) // LANG_ko_KR
        {

            secStr = tr("STR_UPGRADE_SECOND");

            QString secStrD = QString(secStr).arg(remainSecond);


            label->setText(completeStr + "\n" + secStrD);

        }else{
            label->setText(completeStr + "\n" /*+ rebootStr + " "*/ +
                                            QString::number(remainSecond) + " " + secStr);
        }


	if ( remainSecond == 0){
		timer->stop();
		rebootClicked();
	}
}


