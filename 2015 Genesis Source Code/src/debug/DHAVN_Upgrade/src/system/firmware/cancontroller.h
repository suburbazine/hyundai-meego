#ifndef	CAN_CONTROLLER_H_
#define	CAN_CONTROLLER_H_

#include <QThread>

class CanController : public QThread{
Q_OBJECT
public:
	enum CanDevice {
		e_CCP, e_RRC, e_FRONT_LCD, e_RL_LCD, e_RR_LCD
	};
	CanController(QObject * obj = 0);
	~CanController();

	void setStopFlagByExternal();

protected:
	void run();

private:
	bool m_stopLoop;

signals:
	void canDataArrived(int, const QByteArray &);

private slots:	
	void setStopFlag();
};


#endif
