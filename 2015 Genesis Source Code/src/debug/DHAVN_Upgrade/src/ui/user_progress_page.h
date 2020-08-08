#ifndef	USER_PROGRESS_PAGE_H_
#define	USER_PROGRESS_PAGE_H_

#include <QWidget>
#include <QLabel>

class PushButton;
class SubTitle;

class UserProgressPage : public QWidget{
Q_OBJECT
public:
	UserProgressPage(QWidget * p = 0);
	~UserProgressPage();

	void setAllUpgradeCount(int count, unsigned int weight);
	void setStatus(const QString &, const QString &, int progress, int weight);
	void setStatusDirect(const QString &, const QString &, int progress, int weight);
protected:
	void paintEvent(QPaintEvent *);
	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);

private:
	typedef struct{
		QString name;
		int progress;
		int weight;
	} ProgressStatus;

	int m_updateCount;
	int progress;
	int m_weight;


	QPixmap	bgPixmap;
	QPixmap progressPixmap;

	QLabel * titleLabel;
	QLabel * textLabel1;
	QLabel * textLabel2;

	ProgressStatus * progressStatus;
	SubTitle * subTitle;

	int findProgress(const QString & name);

	QString startingStr;
	QString terminatingStr;
	bool m_modeTrigger;

signals:
	void modeChange();
};

#endif
