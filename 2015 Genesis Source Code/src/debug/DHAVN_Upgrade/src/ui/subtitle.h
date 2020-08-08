#ifndef	SUBTITLE_H_
#define	SUBTITLE_H_

#include <QWidget>
#include <QLabel>

class PushButton;

class SubTitle : public QWidget{
Q_OBJECT
public:
	SubTitle(QWidget * p = 0);

private:
	QLabel * m_titleLabel;
};

#endif
