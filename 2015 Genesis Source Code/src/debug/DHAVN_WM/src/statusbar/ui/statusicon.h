#ifndef	STATUS_ICON_H_
#define	STATUS_ICON_H_

#include <QWidget>

class QTimer;

class StatusIcon : public QWidget{
Q_OBJECT
public:
	StatusIcon(QWidget * p = 0);

	void setIconString(const QString &);
	const QString & getIconString();
	void clear();

	static int IconIndex(const QString &);

protected:
	int  getAnimationIndex() const { return animationIndex; }

private:
	QString iconString;
	QTimer * timer;
	int		index;
	int		animationIndex;

protected:
	void paintEvent(QPaintEvent *);
	void hideEvent(QHideEvent *);
};

#endif
