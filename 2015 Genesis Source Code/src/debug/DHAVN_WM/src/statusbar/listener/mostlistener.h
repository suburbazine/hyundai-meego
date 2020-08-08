#ifndef DHAVN_MOSTLISTENER_H
#define DHAVN_MOSTLISTENER_H

#include <QThread>
#include <QObject>
#include <QTimer>
#include <QDBusVariant>

class MOSTListener: public QObject
{
   Q_OBJECT
public:
	MOSTListener( QObject *parent = NULL );
	~MOSTListener();

private:
	enum Control {
		ControlNone = 0x0,	// None
		ControlOneShot = 0x1,
		ControlOnGoing = 0x2,
		ControlClear = 0x11	
	};
	inline bool checkValidOSD(uchar target, uchar osdId, uchar osdControl, uchar osdColor);

        bool mScreenSwap;
        int mClone;
public:
	void connectToMOST();
	inline int getDisplay(uchar target);
	inline QString modeToString(uchar);


        bool getScreenSwap();
        int getClone();

private slots:
	void OnDisplayOSD(const QDBusVariant &);
	void parseDisplayOSD(uchar target, uchar osdId, uchar osdControl,
							uchar osdMode, uchar textColor, QString osdText);
        void setScreenSwap(bool swap);
        void setClone(int clone);
signals:
	void clearOSD(uchar);
	void osdCommand(int, const QString &, const QStringList &, uchar);
};

#endif // DHAVN_MOSTLISTENER_H
