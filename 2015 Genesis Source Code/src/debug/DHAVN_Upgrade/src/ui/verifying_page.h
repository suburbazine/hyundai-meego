#ifndef	VERIFYING_PAGE_H_
#define	VERIFYING_PAGE_H_

#include <QWidget>
#include <QTimer>
#include <QPainter>

class SubTitle;

#define ANI_IMAGE_COUNT		16

class PushButton;

class VerifyingPage : public QWidget{
Q_OBJECT
public:
	VerifyingPage(QWidget * w = 0);
	~VerifyingPage();

protected:
	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

protected:
	void paintEvent(QPaintEvent *);

private:
	QPixmap * pixmap;
	QTimer * timer;
    QTimer * v_timer;//MOBILUS_JWYANG_cancel 팝업 2초 후 활성화 되도록 수정
	int		index;
	PushButton * pushButton;
	SubTitle * subTitle;

signals:
	void cancel();

private slots:
    void jogDialEvent(int);
    void inittimer();//MOBILUS_JWYANG_cancel 팝업 2초 후 활성화 되도록 수정
};

#endif
