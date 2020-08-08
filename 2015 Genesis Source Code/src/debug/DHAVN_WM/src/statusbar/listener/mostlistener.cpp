#include "mostlistener.h"
#include <QMetaType>
#include "common.h"
#include "dhavnlog.h"
#include "DHAVN_MOSTManager_MOSTStructures.h"

#define MOST_MANAGER_SERVICE_NAME           "com.dhavn.mostmanager.qtdbus"
#define MOST_MANAGER_HMI_PATH                "/hmi"
#define MOST_MANAGER_HMI_INTERFACE           ""

//#ifdef	IBOX_OSD_TEST
//#include "test_ibox_sender.h"
//#include <QCoreApplication>
//#include <QDBusConnection>
//#endif


MOSTListener::MOSTListener(QObject *parent):
    QObject( parent )
{
	qDBusRegisterMetaType<SDisplayOSD>();
//#ifdef	IBOX_OSD_TEST
//	TestIBoxSender * t = new TestIBoxSender(QCoreApplication::instance());
//	connect(t, SIGNAL(msgArrived(uchar, uchar, uchar, uchar, uchar, QString)), SLOT(parseDisplayOSD(uchar, uchar, uchar, uchar, uchar, QString)));

//	QDBusConnection sessionBus = QDBusConnection::sessionBus();
//	if ( sessionBus.isConnected()){
//		if (sessionBus.registerService("com.lge.car.iboxtest")){
//			sessionBus.registerObject("/", QCoreApplication::instance());
//		}
//	}
//#endif
    mScreenSwap = false;
    mClone = 0;
}

MOSTListener::~MOSTListener()
{
}

void MOSTListener::connectToMOST()
{
   QDBusConnection sessionBus = QDBusConnection::sessionBus();

   if (!sessionBus.isConnected())
   {
	  lprintf("FatalError : Failed to connect MOST Manager");
      return;
   }

   /** connect signals */
	sessionBus.connect(MOST_MANAGER_SERVICE_NAME, MOST_MANAGER_HMI_PATH, MOST_MANAGER_HMI_INTERFACE, "DisplayOSD", this, SLOT(OnDisplayOSD(const QDBusVariant &)));
}
/*
struct SDisplayOSD
{
    uchar target;
    uchar osdId;
    uchar osdControl;
    uchar osdMode;
    uchar textColor;
    QString osdText;
};
*/

void MOSTListener::OnDisplayOSD(const QDBusVariant & var){
    struct SDisplayOSD sDisplayOSD;

    var.variant().value<QDBusArgument>() >> sDisplayOSD;
	parseDisplayOSD(sDisplayOSD.target, sDisplayOSD.osdId, sDisplayOSD.osdControl, sDisplayOSD.osdMode, sDisplayOSD.textColor, sDisplayOSD.osdText);
}


void MOSTListener::parseDisplayOSD(uchar target, uchar osdId, 
	uchar osdControl, uchar osdMode, uchar textColor, QString osdText){
	lprintf("parseDisplayOSD : target(%02X), osdId(%02X), osdControl(%02X), osdMode(%02X), textColor(%02X), %s", target, osdId, osdControl, osdMode, textColor, qPrintable(osdText));

	if ( !checkValidOSD(target, osdId, osdControl, textColor)){
		lprintf("Invalid IBOX OSD Request : %02X, %02X, %02X", target, osdId, osdControl);
		return;
	}
	
	if ( osdControl == ControlClear ){
		emit clearOSD(osdId);
	}
	else{
		QString modeString = modeToString(osdMode);
		if ( modeString.isEmpty()){
			lprintf("IBOX Unknown Mode : %02X", osdMode);
			return;
		}

		QString option = (osdControl == ControlOnGoing ? "STAY":"");
		QStringList commands = osdText.split("\t");
		commands.prepend( (textColor == 0x02 ? "TUNE":"INFO")); 
		commands.prepend(modeString);
		commands.prepend("IBOX");

		emit osdCommand(getDisplay(target), option, commands, osdId);
	}	

}

inline int MOSTListener::getDisplay(uchar target){
	int display = 0;
        int value = getClone();

	switch(target){
	case 0x00: 
		display = WM_REAR_VAL | WM_FRONT_VAL;
		break;
        case 0x01:
                if (value == 0) {
                    if (getScreenSwap() == false)
                        display = WM_FRONT_VAL;
                    else
                        display = WM_REAR_VAL;
                }
                else if (value == 1) {
                    display = WM_FRONT_VAL;
                }
                else if (value == 2) {
                    display = WM_REAR_VAL;
                }
		break;
	case 0x02:
	case 0x03:
                if (value == 0) {
                    if (getScreenSwap() == false)
                        display = WM_REAR_VAL;
                    else
                        display = WM_FRONT_VAL;
                }
                else if (value == 1) {
                    display = WM_FRONT_VAL;
                }
                else if (value == 2) {
                    display = WM_REAR_VAL;
                }
		break;
	default:
		break;
	}
        lprintf("MOSTListener::getDisplay display:%d",display);
	return display;
}

inline bool MOSTListener::checkValidOSD(uchar target, uchar osdId, uchar osdControl, uchar osdColor){
	if ( target > 0x03 ) return false;
	else if ( osdId == 0x0) return false;
	else if ( osdControl != ControlOneShot &&
				osdControl != ControlOnGoing &&
				osdControl != ControlClear)
		return false;
	else if ( osdColor > 0x02) return false;

	return true;
}

inline QString MOSTListener::modeToString(uchar mode){
	switch(mode){
	case 0x00: return "COMMON";
	case 0x12:
	case 0x13:
	case 0x14:
		return "RADIO";
	case 0x23:
	case 0x24:
	case 0x25:
	case 0x26:
		return "DMB";
	case 0x31:
		return "SXM";
	case 0x40:
	case 0x4B:
		return "DISC";
	case 0x61:
		return "IPOD";
	case 0x63:
		return "AUX";
	case 0x64:
	case 0x65:
	case 0x66:
		return "USB";
	case 0x67:
	case 0x68:
		return "AUX";
	case 0x71:
	case 0x72:
	case 0x73:
		return "JUKEBOX";
	case 0x86:
		return "BLUETOOTH";
	case 0x90:
		return "BLUELINK";
	case 0xB0:
	case 0xB1:
	case 0xB2:
	case 0xB3:
		return "DLNA";
	case 0xC1:
		return "APPLICATION";
	default:
		break;
	}
	return "";
}

void MOSTListener::setScreenSwap(bool swap)
{
    if (swap == true) {
        lprintf("MOSTListener set swap true");
    }
    else {
        lprintf("MOSTListener set swap false");
    }
    mScreenSwap = swap;
}

bool MOSTListener::getScreenSwap()
{
    if (mScreenSwap == true) {
        lprintf("MOSTListener get swap true");
    }
    else {
        lprintf("MOSTListener get swap false");
    }
    return mScreenSwap;
}

void MOSTListener::setClone(int clone)
{
    lprintf("MOSTListener setClone:"+clone);
    mClone = clone;
}

int MOSTListener::getClone()
{
    lprintf("MOSTListener getClone:"+mClone);
    return mClone;
}
