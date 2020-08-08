#ifndef	NO_UPGRADE_PAGE_H_
#define NO_UPGRADE_PAGE_H_

#include <QWidget>
#include <QLabel>

class PushButton;
class SubTitle;

class NoUpgradePage : public QWidget{
Q_OBJECT
public:
	NoUpgradePage(QWidget * p = 0);
	~NoUpgradePage();

	void		setText(const QString &);

private:
	PushButton * okButton;
	QLabel *	 label;
	SubTitle *	 subTitle;

private slots:
	void jogDialEvent(int);

signals:
	void cancelConfirm();
};

#endif
