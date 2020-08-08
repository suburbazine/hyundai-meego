#ifndef	FAIL_PAGE_H_
#define FAIL_PAGE_H_

#include <QWidget>
#include <QTimer>
#include "common.h"

class PushButton;
class QTextEdit;
class QLabel;

class FailPage : public QWidget{
Q_OBJECT
public:
	FailPage(QWidget * p = 0);
	~FailPage();

	void appendFailList(const QString &);

	void setRebootButton(bool isEnable);
private:
	QTextEdit * textEdit;
	PushButton * rebootButton;
	QTimer * timer;
	int remainSecond;

	bool		rebootButtonEnabled;

signals:
	void rebootNow();

private slots:
	void jogDialEvent(int);
};



#endif
