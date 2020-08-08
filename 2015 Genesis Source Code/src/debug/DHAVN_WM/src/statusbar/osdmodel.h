#ifndef	OSD_MODEL_H_
#define	OSD_MODEL_H_

#include <QObject>
#include <QTimer>
#include <QStringList>
#include "osddata.h"

#define OSD_TIMEOUT 5000

class OSDView;

class OSDModel : public QObject{
Q_OBJECT
public:
	enum ClearType{
		ClearAll,
		ClearAV,
		ClearBluetooth,
		ClearBluelink,
		ClearClimate
	};

	OSDModel(bool isFront, QObject * parent = 0);
	void setView(OSDView * view);

private:
	OSDView *		m_view;
	bool			m_front;

	QString			 m_currentScreen;
	QList<OSDData *> m_dataStack;

	QStringList		m_avScreenList;
	QStringList		m_bluetoothScreenList;
	QStringList		m_bluelinkScreenList;
	QStringList		m_infoScreenList;

	QStringList		m_avCommandList;
	QStringList		m_bluetoothCommandList;
	QStringList		m_infoCommandList;

        QStringList		m_avCommandClearList;

	bool			checkShowOSD(int d);

	void			deleteNormalOSDs();

	bool			m_activate;

	OSDData *		isStackContainStay(const QString & cmd);
	int				getIndexStack(const QString & cmd);

	int				getBluetoothStayIndex();
	int				getBluelinkStayIndex();

	int				getIndexById(uchar id);

	OSDData *		m_currentOSD;

	void			processNormalOSD(const QStringList &, uchar);
	void			processStayOSD(const QStringList &, uchar);

	void			processOSD(int prevCount);

	void			processDuplicateOSD(OSDData * data, bool isIncoming);

	void			clear(const QStringList & list);
	void			clearCommands(const QStringList & cmd);
	void 			clearAV();
	void 			clearBluetooth();
	void 			clearInfo();
        void clearAV_exceptDMB();

private slots:
	void processOSD(int, const QString &, const QStringList &, uchar);


	void osdTimeout();

	void activateOSD();

	void lockOSD(bool, bool);

	void clearAll();

	void checkScreen(bool, const QString &);

	void clearById(uchar);

signals:
	void osdEmpty();
};


#endif
