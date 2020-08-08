#ifndef	STATUS_BAR_MODEL_H_
#define	STATUS_BAR_MODEL_H_

#include <QObject>
#include <QStringList>

#include "powerstatuslistener.h"

class Clock;

class StatusBarModel : public QObject
{
	Q_OBJECT
public:
	static StatusBarModel * instance(){
		if ( statusBarModel == NULL)
			statusBarModel = new StatusBarModel;

		return statusBarModel;
	}

	bool isTA() { return m_taEnabled; }
	bool isNEWS() { return m_newsEnabled; }
	bool isREG() { return m_regEnabled; }
        bool isPowerOn();

	Clock * getClock() { return m_clock; }

private:	
	StatusBarModel(QObject * obj = 0);

	static StatusBarModel * statusBarModel;

	bool m_taEnabled;
	bool m_newsEnabled;
	bool m_regEnabled;

	Clock * m_clock;
        PowerStatusListener * m_powerStatusListener;

public slots:
	void statusChanged(const QStringList &);
	void eventChanged(const QStringList &);

signals:
	void updateIconView();
	void updateEuropeInfo();

	void timeChanged(int chour, int cminute, const QString & cAMPM);

        void powerStatusChanged();
};

#endif
