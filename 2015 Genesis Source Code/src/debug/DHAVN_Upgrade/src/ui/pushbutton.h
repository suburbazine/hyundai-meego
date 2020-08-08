#ifndef	PUSH_BUTTON_H_
#define	PUSH_BUTTON_H_

#include <QWidget>
#include <QPainter>

class PushButton : public QWidget{
Q_OBJECT
public:
	PushButton(const QString & str, QWidget * w);
	PushButton(QWidget * w, bool loadPixmap = true);	// for subtitle
	~PushButton();

	void setFocus(bool);
	void enter();
	void release();
	bool isFocused() { return focused; }
	void setText(const QString &);

	void setNormalPixmap(const QString & str);
	void setFocusPixmap(const QString & str);
	void setPressPixmap(const QString & str);

protected:
	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void paintEvent(QPaintEvent *);
private:
	QString text;
	bool	pressed;
	bool	focused;

	QPixmap	pressPixmap;
	QPixmap	normalPixmap;
	QPixmap	focusPixmap;
	QPixmap	focusPressPixmap;

signals:
	void clicked();
};

#endif
