#ifndef	DEV_PROGRESS_PAGE_H_
#define	DEV_PROGRESS_PAGE_H_

#include <QAbstractScrollArea>
#include <QEvent>

class ProgressWidget;

typedef struct{
	QString name;
	QString status;
	int progress;
} ProgressStatus;

class DevProgressPage : public QWidget{
Q_OBJECT
public:
	DevProgressPage(QWidget * p = 0);
	void setAllUpgradeCount(int);

public slots:
	void setStatus(const QString & name, const QString & status, int progress);

protected:
	void paintEvent(QPaintEvent *);

private:
	QList<ProgressStatus *> list;
	QList<ProgressWidget *> widgetList;
	int allUpgradeCount;
	int currentIndex;
	int rowHeight;
/*
private slots:
	void updateLayout();
*/
};

#endif
