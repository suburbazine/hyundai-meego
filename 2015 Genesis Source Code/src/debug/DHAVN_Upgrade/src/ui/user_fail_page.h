#ifndef	USER_FAIL_H_
#define USER_FAIL_H_

#include <QWidget>

class PushButton;
class QLabel;
class QTimer;

class UserFailPage : public QWidget{
Q_OBJECT
public:
	UserFailPage(QWidget * p = 0);
	~UserFailPage();

	void setRebootButton(bool canPress);

private:
	QLabel * m_label;
	PushButton * m_rebootButton;
	QTimer * m_timer;
	int		 m_remainSecond;
	bool	 m_buttonEnable;

protected:
	void showEvent(QShowEvent *);

signals:
	void rebootNow();

private slots:
	void rebootClicked();
	void decreaseSec();

	void jogDialEvent(int);
};

#endif
