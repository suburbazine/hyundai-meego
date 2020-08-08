#ifndef	UPGRADE_WIDGET_H_
#define	UPGRADE_WIDGET_H_

#include <QWidget>
#include <QPainter>
#include <QStackedWidget>
#include "verifying_page.h"
#include "no_upgrade_page.h"
#include "dev_progress_page.h"
#include "user_progress_page.h"
#include "finish_page.h"
#include "fail_page.h"
#include "user_fail_page.h"

class QLabel;

class UpdateWidget : public QWidget{
Q_OBJECT
public:
	UpdateWidget(QWidget * w = 0);
	~UpdateWidget();


private:
	VerifyingPage * 	m_verifyingPage;	
	NoUpgradePage *		m_noUpgradePage;

	DevProgressPage *	m_devProgressPage;
	UserProgressPage *	m_userProgressPage;

	FinishPage *		m_finishPage;
	UserFailPage *		m_userFailPage;

	FailPage *			m_failPage;

	QStackedWidget * 	m_stackWidget;

	QWidget *			m_statusBar;

	int					m_user;

	bool				m_waitReboot;

private slots:
	void availableUpdate(int, unsigned int);
	void goToProgressPages();

	void progressChanged(const QString &, const QString &, unsigned int, unsigned int);
	void progressChangedDirect(const QString &, const QString &, unsigned int, unsigned int);

	void imageValidateFailed();
	void updateFinished(bool, const QString &);
	void goToVerifyPage();

	void usbDetached();
	void setUser(int);

	void checkModeChange();
public slots:
	void failToUpdate(const QString & msg);
	void goToFailPage();

signals:
	void startReboot();
	void stopUpdate();
        void startRebootFailCase(); 
	void jogDialEvent(int);
};


#endif
