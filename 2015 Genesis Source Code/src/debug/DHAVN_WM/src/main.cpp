#include <QApplication>
#include <QFont>
#include <QStringList>
#include "dhavnlog.h"
#include "dhavnwm.h"
#include "osdview.h"
#include "messageprocess.h"
#include "messageserver.h"
#include "osdmodel.h"
#include "mostlistener.h"
#include "common.h"

extern void get_system_info(int &, int &, int &);
extern void apply_language_info(int, int);

int variant = 0;

int main(int argc, char ** argv){
	open_log("/log_data/log/dhavn_wm");
	QApplication app(argc, argv);


//	QStringList fontListA;
//	fontListA << "NewHDB" << "HDB";
//	QFont::insertSubstitutions(QString("REGULAR_HDB"), fontListA);
//	QApplication::setFont(QFont("REGULAR_HDB"));

//	QStringList fontListB;
//	fontListB << "NewHDB" << "HDB" << "DFHeiW5-A";
//	QFont::insertSubstitutions(QString("CHINESE_HDB"), fontListB);

        QStringList fontList1;
        fontList1 << "NewHDB_CN" << "Amiri" << "DFHeiW5-A";
        QFont::insertSubstitutions(QString("DH_HDB"), fontList1);
        QApplication::setFont(QFont("DH_HDB"));

	MessageProcess process;
	MessageServer server;
	server.startServer();
	QObject::connect(&server, SIGNAL(messageArrived(const QString &)), &process, SLOT(process(const QString &)));

	// Broadcast Message
	QObject::connect(&process, SIGNAL(broadcastCommand(const QString &)), &server, SLOT(sendBroadcastMessage(const QString &)));

	int language = 0;
	int rseStatus = 0;

	get_system_info(variant, language, rseStatus);
	apply_language_info(variant, language);		

	if ( rseStatus )
		g_status_table[0].show = true;

	DHAVNWm wm;
	wm.start();

	OSDView frontOSD(variant);
	OSDView rearOSD(variant);

        QObject::connect(&server, SIGNAL(messageArrived(const QString &)), &frontOSD, SLOT(logOSD(const QString &)));
	OSDModel frontOSDModel(true);
	OSDModel rearOSDModel(false);

	rearOSD.setRearScreen();

	frontOSD.setLanguage(language);
	rearOSD.setLanguage(language);

	wm.setOSD(frontOSD.winId(), rearOSD.winId());

	frontOSDModel.setView(&frontOSD);
	rearOSDModel.setView(&rearOSD);

	// LANGUAGE CHANGE
	QObject::connect(&process, SIGNAL(languageChanged(int)), &frontOSD, SLOT(setLanguage(int)));
	QObject::connect(&process, SIGNAL(languageChanged(int)), &rearOSD, SLOT(setLanguage(int)));

	// OSD LCD Request
	 QObject::connect(&frontOSDModel, SIGNAL(osdEmpty()), &process, SLOT(statusBarHide()));

	// Message Process -> OSD
	QObject::connect(&process, SIGNAL(osdCommand(int, const QString &, const QStringList &, uchar)), &frontOSDModel, SLOT(processOSD(int, const QString &, const QStringList &, uchar)));
	QObject::connect(&process, SIGNAL(osdCommand(int, const QString &, const QStringList &, uchar)), &rearOSDModel, SLOT(processOSD(int, const QString &, const QStringList &, uchar)));

	// OSD Lock
	QObject::connect(&process, SIGNAL(osdLock(bool,bool)), &frontOSDModel, SLOT(lockOSD(bool, bool)));
	QObject::connect(&process, SIGNAL(osdLock(bool,bool)), &rearOSDModel, SLOT(lockOSD(bool, bool)));

	// OSD Clear(Power OFF)
	QObject::connect(&process, SIGNAL(clearOSDAll()), &frontOSDModel, SLOT(clearAll()));
	QObject::connect(&process, SIGNAL(clearOSDAll()), &rearOSDModel, SLOT(clearAll()));
	QObject::connect(&process, SIGNAL(clearOSDFront()), &frontOSDModel, SLOT(clearAll()));

	// Activate
//	QObject::connect(&wm, SIGNAL(firstActivated()), &frontOSDModel, SLOT(activateOSD()));
        QObject::connect(&wm, SIGNAL(agreed()), &frontOSDModel, SLOT(activateOSD()));
	QObject::connect(&wm, SIGNAL(agreed()), &rearOSDModel, SLOT(activateOSD()));
	// Notify Agreed
	QObject::connect(&wm, SIGNAL(agreed()), &server, SLOT(sendAgreed()));
	QObject::connect(&wm, SIGNAL(backToAgree()), &server, SLOT(sendBackAgreed()));

	// Screen Change 
	QObject::connect(&wm, SIGNAL(screenChanged(bool, const QString &)), &frontOSDModel, SLOT(checkScreen(bool, const QString &)));
	QObject::connect(&wm, SIGNAL(screenChanged(bool, const QString &)), &rearOSDModel, SLOT(checkScreen(bool, const QString &)));


	// IBOX DisplayOSD
	MOSTListener listener;
	QObject::connect(&listener, SIGNAL(clearOSD(uchar)), &frontOSDModel, SLOT(clearById(uchar)));
	QObject::connect(&listener, SIGNAL(clearOSD(uchar)), &rearOSDModel, SLOT(clearById(uchar)));
	QObject::connect(&listener, SIGNAL(osdCommand(int, const QString &, const QStringList &, uchar)), &frontOSDModel, SLOT(processOSD(int, const QString &, const QStringList &, uchar)));
	QObject::connect(&listener, SIGNAL(osdCommand(int, const QString &, const QStringList &, uchar)), &rearOSDModel, SLOT(processOSD(int, const QString &, const QStringList &, uchar)));
        QObject::connect(&process, SIGNAL(screenSwap(bool)), &listener, SLOT(setScreenSwap(bool)));
        QObject::connect(&process, SIGNAL(screenClone(int)), &listener, SLOT(setClone(int)));

        listener.connectToMOST();

	return app.exec();
}
