#ifndef	INFO_ICON_H_
#define	INFO_ICON_H_

#include <QWidget>

class QTimer;

class InfoIcon : public QWidget{
Q_OBJECT
public:
	InfoIcon(QWidget * p = 0);

	void setIcon(int idx);
	void clear();

private:
	QTimer * timer;
	int		index;
	int		animationIndex;

protected:
	void paintEvent(QPaintEvent *);
	void hideEvent(QHideEvent *);
};

#endif
