#include "messageevent.h"
#include "msg_client.h"
#include "common.h"

typedef struct{
	int id;
	QString package_name;
	QString display_name;
} ModuleTable;


static ModuleTable moduleTables[] = {
	{ MID_BLUETOOTH,	"bt",		"FW BT" },
	{ MID_CCP,			"ccp",		"FW CCP"},
	{ MID_RRC,			"rrc",		"FW RRC"},
	{ MID_DAB,			"dab",		"FW DAB"},
	{ MID_FPGA,			"fpga",		"FW FPGA"},
	{ MID_INIC,			"inic",		"FW INIC"},
	{ MID_TDMB,			"tdmb",		"FW TDMB"},
	{ MID_MICOM,		"micom", 	"FW MICOM"},
	{ MID_HDRADIO,		"hdradio",	"FW HDR"},
	{ MID_GPS,			"gps",		"FW GPS"},
	{ MID_XM,			"xm",		"FW XM"},
	{ MID_BIOS,			"bios",		"FW BIOS"},
	{ MID_FRONT_LCD,	"front_lcd","FW F_LCD"},
	{ MID_REAR_LEFT_LCD, "rear_left_lcd", "FW RL_LCD"},
        { MID_REAR_RIGHT_LCD, "rear_right_lcd", "FW RR_LCD"},
        { MID_SSD, "ssd", "FW SSD"},
	{ MID_GNSS,			"gnss",		"FW GNSS"},
	{ -1, "", ""}	
};

MessageEvent::MessageEvent(QObject * obj)
: QObject(obj){
	server = new MessageServer(this);
	connect(server, SIGNAL(messageArrived(int, int, int, const QString &)), SLOT(receiveEvent(int, int, int, const QString &)));
	server->startServer();
}

MessageEvent::~MessageEvent(){

}

void MessageEvent::receiveEvent(int id, int progress, int status, const QString & msg){
	static int old_id = -1;
	static int old_progress = -1;
	static int old_status = -1;
	static QString old_msg("xxx");

	ULOG->log("MessageEvent:receiveEvent %s, %d, %d %s", 
			qPrintable(GetModuleIdToString(id)), progress, status, (msg.isEmpty()?"nil":qPrintable(msg)));

	if ( old_id != id || old_progress != progress || status != old_status || old_msg != msg){
		old_id = id;
		old_progress = progress;
		old_status = status;
		old_msg = msg;
		
		emit messageProgressRaw(id, status, progress, getModuleStatusToString(status, msg));

		if ( status != MODULE_STATUS_INFO){
			emit messageProgress( GetModuleIdToString(id),
				getModuleStatusToString(status, msg),
				progress);	
		}
	}
}

QString MessageEvent::GetModuleIdToString(int id){
	int index = 0;
	while (1){
		if ( moduleTables[index].id == -1)
			break;

		if ( moduleTables[index].id == id)
			return moduleTables[index].display_name;

		index++;
	}
	return "FW-UNKNOWN";
}

QString MessageEvent::GetModuleIdToPackageName(int id){
	int index = 0;

	while(1){
		if ( moduleTables[index].id == -1)
			break;

		if ( moduleTables[index].id == id)
			return moduleTables[index].package_name;

		index++;
	}
	
	return "";
}

QString MessageEvent::GetModulePackageNameToDisplay(const QString & packageName){
	int index = 0;
	while(1){
		if ( moduleTables[index].id == -1)
			break;

		if ( moduleTables[index].package_name == packageName.toLower())
			return moduleTables[index].display_name;

		index++;
	}

	return "";
}


QString MessageEvent::getModuleStatusToString(int status, const QString & msg){
	QString statusStr;
	switch( (ModuleStatus)status){
	case MODULE_STATUS_INIT:
		statusStr = "Preparing"; break;
	case MODULE_STATUS_PROGRESS:
		statusStr = "Updating"; break;
	case MODULE_STATUS_WARNING:
		statusStr = "WARNING!"; break;
	case MODULE_STATUS_RETRY:
		statusStr = "RETRY!"; break;
	case MODULE_STATUS_FAIL:
		statusStr = "FAIL!"; break;
	case MODULE_STATUS_DONE:
		statusStr = "DONE"; break;
	case MODULE_STATUS_OLD_VERSION:
		return "No need to update";
	default:
		break;
	}
	statusStr.append( QString(" : ") + msg);
	return statusStr;
}
