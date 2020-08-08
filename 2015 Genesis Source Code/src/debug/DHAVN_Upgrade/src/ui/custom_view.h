#ifndef	CUSTOM_VIEW_H_
#define	CUSTOM_VIEW_H_

#include <QDeclarativeView>

class CustomView : public QDeclarativeView{
Q_OBJECT
public:
	CustomView(QWidget * p = 0);
};

#endif
