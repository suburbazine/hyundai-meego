#ifndef	ICON_WIDGET_H_
#define	ICON_WIDGET_H_

#include <QWidget>
#include <QTimer>


class IconWidget : public QWidget{
Q_OBJECT
public:
	IconWidget(QWidget * p = 0);
	void setIndex(int);

protected:
	void paintEvent(QPaintEvent *);

private:
	int m_index;
	int m_value;
	int m_animationIndex;
	QTimer * timer;
};


#endif
