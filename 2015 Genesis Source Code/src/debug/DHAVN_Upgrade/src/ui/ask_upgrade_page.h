#ifndef	ASK_UPGRADE_PAGE_H_
#define ASK_UPGRADE_PAGE_H_

#include <QWidget>
#include <QLabel>

class PushButton;
class SubTitle;

class AskUpgradePage : public QWidget{
Q_OBJECT
public:
	AskUpgradePage(QWidget * p = 0);
	~AskUpgradePage();

private:
	PushButton * okButton;
	PushButton * cancelButton;
	QLabel *	 label;
	SubTitle *	 subTitle;

private slots:
	void jogDialEvent(int);

signals:
	void upgradeConfirm();
	void upgradeCancel();

};


#endif
