#ifndef	ALERT_DIALOG_H_
#define	ALERT_DIALOG_H_

#include <QWidget>
#include <QLabel>

class PushButton;

class AlertDialog : public QWidget{
Q_OBJECT
public:
	AlertDialog(QWidget * p);

private:
	QLabel * label;
	PushButton * button;

signals:
	void exitConfirmed();

private slots:
	void jogDialEvent(int);
};

#endif
