#ifndef	OSD_CLIMATE_H_
#define	OSD_CLIMATE_H_

#include "abstract_osd_draw.h"
#include "airwidget.h"

class OSDClimate : public AbstractOSDDraw{
Q_OBJECT
public:
	OSDClimate(QPixmap *, int variant = 0);
	bool processCommand(const QStringList & args);

	int getMinimumArgCount();

	const QStringList & getCommand();

	void setUIPosition();

private:
	int		 m_prevPassengerTempWidth;
	int		 m_prevDriverTempWidth;

	class ClimateInfo{
	public:
		enum Mode{ UNKNOWN, OFF, VENT, BL, FLOOR, MIX, DEF, AUTO_DEF};

		ClimateInfo();

	private:
		QString		driverStr;
		QString		passStr;
                QString		ambientStr;
		QString		driverTempStr;
		QString		passTempStr;
                QString		ambientTempStr;
		Mode		driverMode;
		Mode		passMode;
		QString		autoStr;
		int			mainBlower;
		int			air;
		bool		dualTemp;
		bool		dualMode;
		bool		isBlinking;
	public:
                void		setTemperature(const QString & str, int type);
		void		setMode(const QString & str, bool isDriver);
		void		setAuto(const QString & str);
		void		setBlower(const QString & str);
		void		setDualTemperature(const QString & str);
		void		setDualMode(const QString & str);
		void		setAir(const QString & str);
		void		setBlinking(const QString & str);

		const QString & getDriverStr() { return driverStr; }
		const QString & getDriverTemperatureStr() { return driverTempStr; }
		const QString & getPassengerStr() { return passStr; }
		const QString & getPassengerTemperatureStr() { return passTempStr; }
                const QString & getAmbientStr() { return ambientStr; }
                const QString & getAmbientTemperatureStr() { return ambientTempStr; }

		Mode		getDriverMode() { return driverMode; }
		Mode		getPassengerMode() { return passMode; }

		bool		isDualMode() { return dualMode; }
		bool		isDualTemperature() { return dualTemp; }
		bool		showBliking() { return isBlinking; }

		const QString & getAutoStr() { return autoStr; }

		int			getAir() { return air; }
		int			getMainBlower() { return mainBlower; }
	};

	QStringList command;	

	void	createOff();
	void	createClimate();

	void	showDiag(int);
        void	showOff(const QString &, bool isDualMode = false);
	void	showClimate(ClimateInfo *);

	void	setDriverPosition(const QString &, bool isDualMode = false);
	bool	setModeIcon(ClimateInfo::Mode mode, QLabel *);

	void	hideOff();
	void	hideClimate();
	void	hideDiag();

	QLabel * offIcon;
	QLabel * offText;

	// use climate and diag
	QLabel * driverLabel;
	QLabel * driverTemp;

	QLabel * passengerLabel;
	QLabel * passengerTemp;

        QLabel * ambientLabel;
        QLabel * ambientTemp;

	QLabel * driverModeIcon;
	QLabel * passengerModeIcon;

	QLabel * autoLabel;
	AirWidget * airWidget;
        QLabel * separatorLabel[5];

	QTimer * driverTimer;
	int		 driverTimerIndex;
	QTimer * passengerTimer;
	int		 passengerTimerIndex;
	void	 resetTimer(bool isFront = true);


private slots:
	void driverBlinking();
	void passengerBlinking();
};


#endif
