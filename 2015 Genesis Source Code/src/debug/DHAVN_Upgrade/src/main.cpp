#include <QApplication>
#include <QTranslator>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QFontDatabase>
#include <QStringList>
#include <QMetaType>
#include <QDeclarativeView>
#include <QFile>
#include <QTimer>

#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mount.h>

#include "updatepreparation.h"
#include "update_app_engine.h"
#include "update_widget.h"
#include "update.h"

#include "common.h"
#include "BTAdaptorMethod.h"
#include "reboot.h"
#include <signal.h>

bool usb_status =false;
int  LCD_major =0;
int  LCD_minor =0;
enum LanguageInfo {
	LANG_ko_KR = 1,	// Korea
	LANG_en_US,		// English(US)
	LANG_en_GB,		// English(UK)
	LANG_zh_CN,		// Chinese mandarin
	LANG_zh_HK,		// Chinese cantonese
	LANG_fr_FR,		// French
	LANG_es_ES,		// Spanish
	LANG_ar_SA,		// Arabic
	LANG_cs_CZ,		// Czech
	LANG_da_DK,		// Danish
	LANG_de_DE,		// German
	LANG_it_IT,		// Italian
	LANG_nl_NL,		// Dutch
	LANG_pl_PL,		// Polish
	LANG_pt_PT,		// Portuguese
	LANG_ru_RU,		// Russian
	LANG_sk_SK,		// Slovak
	LANG_sv_SE,		// Swedish
        LANG_tr_TR,		// Turkish
        LANG_fr_CA,             // French(US)
        LANG_es_US              // Spanish(US)

};

static void getLanguageId(int languageId){
	static QTranslator * translator = NULL;

	if ( translator != NULL){
		qApp->removeTranslator(translator);
		delete translator;
	}

	translator = new QTranslator;

	switch( languageId ){
	case LANG_ko_KR:		// Korea
		translator->load(":translation/appupgrade_ko_KR.qm"); break;
	case LANG_en_GB:		// English(UK)
		translator->load(":translation/appupgrade_en_GB.qm"); break;
	case LANG_zh_HK:		// Chinese cantonese
	case LANG_zh_CN:		// Chinese mandarin
		translator->load(":translation/appupgrade_zh_CN.qm"); break;
	case LANG_fr_FR:		// French
		//if ( variant == VARIANT_NA)
		//	translator->load(":translation/appupgrade_fr_CA.qm");
		//else
			translator->load(":translation/appupgrade_fr_FR.qm");
		break;
	case LANG_es_ES:		// Spanish
		//if ( variant == VARIANT_NA)
		//	translator->load(":translation/appupgrade_es_US.qm");
		//else
			translator->load(":translation/appupgrade_es_ES.qm");
		break;
	case LANG_ar_SA:		// Arabic
		translator->load(":translation/appupgrade_ar_YE.qm"); break;
	case LANG_cs_CZ:		// Czech
		translator->load(":translation/appupgrade_cs_CZ.qm"); break;
	case LANG_da_DK:		// Danish
		translator->load(":translation/appupgrade_da_DK.qm"); break;
	case LANG_de_DE:		// German
		translator->load(":translation/appupgrade_de_DE.qm"); break;
	case LANG_it_IT:		// Italian
		translator->load(":translation/appupgrade_it_IT.qm"); break;
	case LANG_nl_NL:		// Dutch
		translator->load(":translation/appupgrade_nl_NL.qm"); break;
	case LANG_pl_PL:		// Polish
		translator->load(":translation/appupgrade_pl_PL.qm"); break;
	case LANG_pt_PT:		// Portuguese
		translator->load(":translation/appupgrade_pt_PT.qm"); break;
	case LANG_ru_RU:		// Russian
		translator->load(":translation/appupgrade_ru_RU.qm"); break;
	case LANG_sk_SK:		// Slovak
		translator->load(":translation/appupgrade_sk_SK.qm"); break;
	case LANG_sv_SE:		// Swedish
		translator->load(":translation/appupgrade_sv_SE.qm"); break;
	case LANG_tr_TR:		// Turkish
		translator->load(":translation/appupgrade_tr_TR.qm"); break;
        case LANG_fr_CA:
                translator->load(":translation/appupgrade_fr_CA.qm"); break;
        case LANG_es_US:
                translator->load(":translation/appupgrade_es_US.qm"); break;
	case LANG_en_US:		// English(US)
	default:
		translator->load(":translation/appupgrade_en_US.qm"); break;
	}
        g_languageId = languageId;
	qApp->installTranslator(translator);
}

static void DeleteArgv(char ** argv){
	for ( char ** i = argv; *i != NULL; ++i)
		free(*i);
	free(argv);
}

static char ** CopyArgv(int argc, char ** argv){
	char ** argvcp = (char **)malloc( (argc + 1) * sizeof(char *));

	for ( int i = 0; i < argc; ++i){
		int len = strlen(argv[i]);
		argvcp[i] = (char *) malloc(sizeof(char *) * (len + 1));
		::strncpy(argvcp[i], argv[i], len);
	}
	argvcp[argc] = NULL;
	return argvcp;
}

static bool isReadonlyPartition(){
	QFile f("/etc/partition-2.8");

	if ( f.exists())
		return true;

	return false;
}

static void sighandler(int sig)
{
  ULOG->log("FAIL SIGHANDLER SIG : %d",sig);
  
  Reboot reboot(false,true);

}

int main(int argc, char ** argv){
	AppEngineBase::SetAppArgs(argc, argv);
	QApplication::setGraphicsSystem("raster");
	char ** argvcp = CopyArgv(argc, argv);
	int argccp = argc;

	QApplication app(argc, argv);


	bool readOnlyRemountResult = true;
	int languageId = -1;
        struct sigaction sa;
        sa.sa_handler = sighandler;
        sigaction(SIGABRT, &sa, NULL);
        sigaction(SIGSEGV, &sa, NULL);
	UpdatePreparation updatePreparation;
//        updatePreparation.tryToKillApplications();
	updatePreparation.sendUpdateInfoToMicom();

	g_isReadonly = isReadonlyPartition();

	if ( g_isReadonly){
		if ( ::mount("/", "/", "ext4", MS_REMOUNT | MS_MGC_VAL, NULL) != 0)	{
			fprintf(stderr, "remount root fs failed\n");
			readOnlyRemountResult = false;
		}

		if ( ::mount("/boot", "/boot", "ext3", MS_REMOUNT | MS_MGC_VAL, NULL) != 0)	{
			fprintf(stderr, "remount boot fs failed\n");
			readOnlyRemountResult = false;
		}
	}
//jaewon.yang_Delete Upgrade_fail
        if(QFile::exists("/etc/upgrade_fail"))
        {
        QFile::remove("/etc/upgrade_fail");
        }

	if (::mkdir(UPGRADE_DIR, 00755) < 0){
		// assume that already exist
		int result = ::chmod(UPGRADE_DIR, 00755);
		Q_UNUSED(result);	// do not care about result
	}

	UpdateLog::instance();

	ULOG->log("AppUpgrade version : 10.2.44");

        // BT app에 연결 해제 요청 -------
        ULOG->log("main BT dbus connect");

        BTAdaptorMethod btDbus;
        QDBusConnection _appCon = QDBusConnection::sessionBus();
        _appCon.registerService("com.lge.car.upgradebt");
        _appCon.registerObject("/", &btDbus);

        btDbus.Request_Disconnect();

        ULOG->log("main BT request disconnect");
        // -----------------------------

	QStringList fontListA;
    fontListA << "NewHDR_CN" << "Amiri" << "DFHeiW5-A";
	QFont::insertSubstitutions(QString("REGULAR_HDR"), fontListA);
	QApplication::setFont(QFont("REGULAR_HDR"));

//	QStringList fontListB;
//	fontListB << "NewHDR" << "HDR" << "DFHeiW5-A" << "Amiri";
//	QFont::insertSubstitutions(QString("CHINESE_HDR"), fontListB);

	DECLARE_META_EVENT();

	::seteuid(MEEGO_USER_ID);
	QDBusConnection conn = QDBusConnection::sessionBus();
	QDBusMessage msg = QDBusMessage::createMethodCall("com.lge.car.micom", "/SysInfo", "com.lge.car.micom.sysinfo", "LgeDBus_AskLanguageInfoValue");
	QDBusMessage variantMsg = QDBusMessage::createMethodCall("com.lge.car.micom", "/SysInfo", "com.lge.car.micom.sysinfo", "LgeDBus_AskSysOption");
	QDBusMessage carTypeMsg = QDBusMessage::createMethodCall("com.lge.car.micom", "/SysInfo", "com.lge.car.micom.sysinfo", "LgeDBus_AskSysVersion");

	QDBusMessage langReply = conn.call(msg, QDBus::Block, 5000);

	if ( langReply.arguments().count()){
		QByteArray langArray = langReply.arguments().at(0).toByteArray();
		if ( langArray.count())
			languageId = (int) langArray.at(0);	
	}

	QDBusMessage variantReply = conn.call(variantMsg, QDBus::Block, 5000);

	if ( variantReply.arguments().count()){
		QByteArray variantArray = variantReply.arguments().at(0).toByteArray();
		if ( variantArray.count() == 9){
			g_variantCode = (int) variantArray.at(1);
			g_variantGetSuccess = true;
		}
	}
	
	QDBusMessage carTypeReply = conn.call(carTypeMsg, QDBus::Block, 5000);
	if ( carTypeReply.arguments().count()){
		QByteArray carTypeArray = carTypeReply.arguments().at(0).toByteArray();
		if ( carTypeArray.count() == 0x1A){
			g_carTypeCode = (int) carTypeArray.at(13);
			ULOG->log("car type : %d", g_carTypeCode);
		}
	}

	QDeclarativeView view;
	UpdateAppEngine uiListener(view);
	view.setAttribute(Qt::WA_NoSystemBackground);
	view.viewport()->setAttribute(Qt::WA_NoSystemBackground);
	uiListener.ConnectToUISH(argccp, argvcp);

	::seteuid(ROOT_USER_ID);
	DeleteArgv(argvcp);

	getLanguageId(languageId);

//	if ( languageId == LANG_zh_CN || languageId == LANG_zh_HK)
//		QApplication::setFont(QFont("CHINESE_HDR"));
//	else
		QApplication::setFont(QFont("REGULAR_HDR"));


	UpdateWidget updateWidget;
	QObject::connect(&uiListener, SIGNAL(jogDialEvent(int)), &updateWidget, SIGNAL(jogDialEvent(int)));

	updateWidget.setGeometry(0, 0, 1280, 720);
	updateWidget.update();
	updateWidget.show();
        updateWidget.raise();

	Update update(&updateWidget);

	if ( readOnlyRemountResult )
		QTimer::singleShot(0, &update, SLOT(start()));
	else
		updateWidget.failToUpdate( QString(QObject::tr("STR_UPGRADE_PREPARE_FAIL")) + " (Partition Fail)");

	return app.exec();
}

