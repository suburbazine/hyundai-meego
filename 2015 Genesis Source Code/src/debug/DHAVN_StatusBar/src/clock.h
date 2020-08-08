#ifndef	CLOCK_H_
#define CLOCK_H_

#include <QObject>

class Clock : public QObject{
	Q_OBJECT

public:
	Clock(QObject * obj = 0) : QObject(obj){
		m_hour = -1;
		m_minute = -1;
	}

	int hour()  { return m_hour; }
	int minute() { return m_minute; }
	const QString & ampm() { return m_AMPM; }

	void setTime(int h, int m, const QString & ampm){
		m_hour = h;
		m_minute = m;
		m_AMPM = ampm;
		
		emit timeChanged(m_hour, m_minute, m_AMPM);
	}

private:
	int m_hour;
	int m_minute;
	QString m_AMPM;

signals:
	void timeChanged(int chour, int cminute, const QString & cAMPM);
};

#endif
