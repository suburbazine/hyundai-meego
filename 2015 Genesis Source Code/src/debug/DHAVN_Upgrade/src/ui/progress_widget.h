#ifndef	PROGRESS_WIDGET_H_
#define	PROGRESS_WIDGET_H_

#include <QWidget>
#include <QLabel>

class ProgressWidget : public QWidget{
public:
	ProgressWidget(const QString & aName, QWidget * p = 0);
	
	void setProgress(int);
	void setName(const QString &);
	void setCurrentStatus(const QString &);

protected:
	void paintEvent(QPaintEvent *);

private:
	int progress;
	QString currentStatus;
	QPixmap bgPixmap;
	QPixmap progressPixmap;

	QLabel * nameLabel;
	QLabel * statusLabel;

signals:
	void changeMode();
};


#endif
