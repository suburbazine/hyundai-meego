#include "user_fail_page.h"
#include "pushbutton.h"

#include <QLabel>
#include <QTimer>
#include "common.h"
UserFailPage::UserFailPage(QWidget * p)
: QWidget(p)
{
	m_remainSecond = 20;
	m_buttonEnable = true;

	m_rebootButton = new PushButton(tr("STR_UPGRADE_RESTART_NOW"), this);
	m_rebootButton->setGeometry(409, 525, 462, 85);
	connect(m_rebootButton, SIGNAL(clicked()), SLOT(rebootClicked()));

	m_label = new QLabel(QString(tr("STR_UPGRADE_NOT_COMPLETE")) + "\n" + tr("STR_UPGRADE_TRY_AGAIN"), this);
        //m_label->setGeometry(150, 200, 980, 160 + 61);
        m_label->setGeometry(0, 200, 1280, 160 + 61);
        m_label->setWordWrap(true);
	m_label->setAlignment(Qt::AlignCenter);

	m_timer = new QTimer(this);
	m_timer->setInterval(1000);
	connect(m_timer, SIGNAL(timeout()), SLOT(decreaseSec()));

	m_rebootButton->setFocus(true);
}

UserFailPage::~UserFailPage(){}

void UserFailPage::showEvent(QShowEvent *){
	if ( !m_buttonEnable && !m_timer->isActive()) m_timer->start();	
}

void UserFailPage::setRebootButton(bool canPress){
	m_buttonEnable = canPress;

	if ( !m_buttonEnable){
		m_rebootButton->hide();
                if ( g_languageId != 1 ) // LANG_ko_KR
                {
                    QString secStr;
                    secStr = tr("STR_UPGRADE_SECOND");
                    QString secStrD = QString(secStr).arg(m_remainSecond);


                    m_label->setText(
                            QString(tr("STR_UPGRADE_NOT_COMPLETE")) +
                                            "\n" +
                                            QString(tr("STR_UPGRADE_TRY_AGAIN")) + "\n" +
                                           // tr("STR_UPGRADE_RESTART") + " " +
                                            secStrD);
                }else{
                    m_label->setText(
                            QString(tr("STR_UPGRADE_NOT_COMPLETE")) +
                                            "\n" +
                                            QString(tr("STR_UPGRADE_TRY_AGAIN")) + "\n" +
                                           // tr("STR_UPGRADE_RESTART") + " " +
                                            QString::number(m_remainSecond) + " " +
                                            tr("STR_UPGRADE_SECOND"));
                }

					

	}
}

void UserFailPage::decreaseSec(){
	m_remainSecond--;
        if ( g_languageId != 1 ) // LANG_ko_KR
        {
            QString secStr;
            secStr = tr("STR_UPGRADE_SECOND");
            QString secStrD = QString(secStr).arg(m_remainSecond);


            m_label->setText(
                            QString(tr("STR_UPGRADE_NOT_COMPLETE")) +
                                            "\n" +
                                            QString(tr("STR_UPGRADE_TRY_AGAIN")) + "\n" +
                                            secStrD);


        }else{
            m_label->setText(
                            QString(tr("STR_UPGRADE_NOT_COMPLETE")) +
                                            "\n" +
                                            QString(tr("STR_UPGRADE_TRY_AGAIN")) + "\n" +
                                           // tr("STR_UPGRADE_RESTART") + " " +
                                            QString::number(m_remainSecond) + " " +
                                            tr("STR_UPGRADE_SECOND"));
        }

					


	if ( m_remainSecond == 0){
		m_timer->stop();
		rebootClicked();
	}
}

void UserFailPage::rebootClicked(){
	emit rebootNow();
}

void UserFailPage::jogDialEvent(int e){
	static bool isFocused = true;

	if (!isVisible())
		return;

	if ( !isFocused){
		isFocused = true;
		m_rebootButton->setFocus(true);
	}
	else{
		if ( e == 0){
			m_rebootButton->enter();
		}	
	}
}
