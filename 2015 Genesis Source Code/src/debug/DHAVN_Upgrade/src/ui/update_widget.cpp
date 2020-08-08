#include "update_widget.h"
#include <QPalette>
#include <QLabel>
#include <QTimer>
#include <stdio.h>
#include <QTextCodec>
#include <QPen>
#include "configuration.h"
#include "alertdialog.h"
#include "updatepreparation.h"
#include "common.h"

extern bool usb_status;
UpdateWidget::UpdateWidget(QWidget * w)
: QWidget(w){
	QPalette pal = palette();
	pal.setBrush(QPalette::Window, QPixmap(":images/bg.png"));
	pal.setColor(QPalette::WindowText, QColor(250, 250, 250));
	setPalette(pal);
	setAutoFillBackground(true);

	QFont f = font();
	f.setPointSize(40);
	setFont(f);

	m_waitReboot = false;
	m_user = -1;

	m_stackWidget=  new QStackedWidget(this);
	m_stackWidget->setGeometry(0, 0, 1280, 720);
	
	m_statusBar = new QWidget(this);
	m_statusBar->setFocusPolicy(Qt::NoFocus);
	pal = m_statusBar->palette();
	pal.setBrush(QPalette::Window, QPixmap(":images/bg_indi.png"));
	pal.setColor(QPalette::WindowText, Qt::darkGray);
	m_statusBar->setPalette(pal);

	m_statusBar->setGeometry(0, 0, 1280, 93);
	m_statusBar->setAutoFillBackground(true);

	QList<QWidget *> focusPages;

	m_verifyingPage = new VerifyingPage;

	m_failPage = new FailPage;
	m_noUpgradePage = new NoUpgradePage;
	m_devProgressPage = new DevProgressPage;
	m_finishPage = new FinishPage;
	m_userProgressPage = new UserProgressPage;
	m_userFailPage = new UserFailPage;
	
	focusPages.append(m_verifyingPage);
	focusPages.append(m_noUpgradePage);
	focusPages.append(m_failPage);
	focusPages.append(m_userFailPage);

	for (int i = 0; i < focusPages.count(); ++i)
		connect(this, SIGNAL(jogDialEvent(int)), focusPages.at(i), SLOT(jogDialEvent(int)));

        //connect(m_noUpgradePage, SIGNAL(cancelConfirm()), SIGNAL(startReboot())); 
        connect(m_noUpgradePage, SIGNAL(cancelConfirm()), SIGNAL(startRebootFailCase()));
	connect(m_verifyingPage, SIGNAL(cancel()), SIGNAL(stopUpdate()));

	connect(m_finishPage, SIGNAL(rebootNow()), SIGNAL(startReboot()));
        //connect(m_userFailPage, SIGNAL(rebootNow()), SIGNAL(startReboot())); 
        connect(m_userFailPage, SIGNAL(rebootNow()), SIGNAL(startRebootFailCase()));
        connect(m_failPage, SIGNAL(rebootNow()), SIGNAL(startReboot()));

	connect(m_userProgressPage, SIGNAL(modeChange()), SLOT(checkModeChange()));

	m_stackWidget->addWidget(m_verifyingPage);
	m_stackWidget->addWidget(m_noUpgradePage);
	m_stackWidget->addWidget(m_devProgressPage);
	m_stackWidget->addWidget(m_userProgressPage);
	m_stackWidget->addWidget(m_failPage);
	m_stackWidget->addWidget(m_finishPage);
	m_stackWidget->addWidget(m_userFailPage);

	goToVerifyPage();
}

UpdateWidget::~UpdateWidget(){
}

void UpdateWidget::goToFailPage(){
	if ( !m_waitReboot ){
		m_noUpgradePage->setText(tr("STR_UPGRADE_NOT_COMPLETE"));
                ::sync(); 
		m_stackWidget->setCurrentWidget(m_noUpgradePage);
		m_waitReboot = true;
	}
}

void UpdateWidget::progressChanged(const QString & name, const QString & desc, unsigned int progress, unsigned int weight){
	switch(m_user){
	case Configuration::DeveloperMode:
		m_devProgressPage->setStatus(name, desc, progress);
		m_userProgressPage->setStatus(name, desc, progress, weight);
		break;
	case Configuration::UserMode:
		m_userProgressPage->setStatus(name, desc, progress, weight);
		break;
	default:
		break;
	}
}

void UpdateWidget::progressChangedDirect(const QString & name, const QString & desc, unsigned int progress, unsigned int weight){
	switch(m_user){
	case Configuration::DeveloperMode:
		m_devProgressPage->setStatus(name, desc, progress);
		m_userProgressPage->setStatusDirect(name, desc, progress, weight);
		break;
	case Configuration::UserMode:
		m_userProgressPage->setStatusDirect(name, desc, progress, weight);
		break;
	default:
		break;
	}
}

void UpdateWidget::setUser(int user){
	m_user = user;
}

void UpdateWidget::goToVerifyPage(){
	m_stackWidget->setCurrentWidget(m_verifyingPage);
}

void UpdateWidget::failToUpdate(const QString & msg){
	if ( !m_waitReboot){
		m_noUpgradePage->setText(msg);
		m_stackWidget->setCurrentWidget(m_noUpgradePage);
                // SANGWOO_TEMP
                UpdatePreparation updatePreparation;
                updatePreparation.tryToKillApplications();
		m_waitReboot = true;
	}
}

void UpdateWidget::imageValidateFailed(){
	if ( !m_waitReboot){
		m_noUpgradePage->setText(tr("STR_UPGRADE_IMAGE_FAILED"));
		m_stackWidget->setCurrentWidget(m_noUpgradePage);
		m_waitReboot = true;
	}
}

void UpdateWidget::availableUpdate(int count, unsigned int weight){
	if ( count == 0)
		return;

	if ( m_user == Configuration::UserMode){
		m_userProgressPage->setAllUpgradeCount(count, weight);
	}
	else{
		m_userProgressPage->setAllUpgradeCount(count, weight);
		m_devProgressPage->setAllUpgradeCount(count);
	}
	goToProgressPages();
}

void UpdateWidget::checkModeChange(){
	if ( m_user == Configuration::DeveloperMode)
		m_stackWidget->setCurrentWidget(m_devProgressPage);
}

void UpdateWidget::usbDetached(){
	if ( !m_waitReboot){
            ULOG->log("###JWYANG_UPDATE_WIDGET usb_status %d",usb_status);
                if(usb_status)
                {
                m_noUpgradePage->setText(tr("STR_UPGRADE_NOT_COMPLETE") + "\n(" + tr("STR_UPGRADE_USB_DETACHED") + ")");
                }
                else
                {
                m_noUpgradePage->setText(tr("STR_UPGRADE_NOT_COMPLETE") + "\n(" + tr("STR_SD_CARD_REMOVE_OSD") + ")");
                }
                m_stackWidget->setCurrentWidget(m_noUpgradePage);
		m_waitReboot = true;
	}
}

void UpdateWidget::goToProgressPages(){
	if ( m_waitReboot ) return;

	switch(m_user){
	case Configuration::DeveloperMode:
	case Configuration::UserMode:
		m_stackWidget->setCurrentWidget(m_userProgressPage);
		break;
	default:
		break;
	}
}

void UpdateWidget::updateFinished(bool canPressButton, const QString & failList){
	if ( !failList.isEmpty()){
		if ( m_user == Configuration::DeveloperMode && m_stackWidget->currentWidget() == m_devProgressPage){
			m_failPage->appendFailList(failList);
			m_stackWidget->setCurrentWidget(m_failPage);
		}
		else{
			m_userFailPage->setRebootButton(canPressButton);
                        ::sync();
                        
			m_stackWidget->setCurrentWidget(m_userFailPage);
		}
	}
	else{
		m_stackWidget->setCurrentWidget(m_finishPage);
	}
}

