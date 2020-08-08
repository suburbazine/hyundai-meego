#ifndef	FINISH_PAGE_H_
#define FINISH_PAGE_H_

#include <QWidget>

class QLabel;
class QTimer;

class FinishPage : public QWidget{
Q_OBJECT
public:
	FinishPage(QWidget * p = 0);
	~FinishPage();

private:
	QLabel * label;
	int		 remainSecond;
	QTimer * timer;

	QString completeStr;
	QString rebootStr;
	QString secStr;

protected:
	void showEvent(QShowEvent *);

signals:
	void rebootNow();

private slots:
	void rebootClicked();
	void decreaseSec();
};

#endif
