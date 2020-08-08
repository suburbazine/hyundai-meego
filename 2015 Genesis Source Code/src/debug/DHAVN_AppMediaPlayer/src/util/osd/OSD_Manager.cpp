#include "OSD_Manager.h"

#include "util/osd/OSD_ScreenManager.h"
#include "controller/DHAVN_AppMediaPlayer_ControllerBase.h"
#include "util/DHAVN_AppMediaPlayer_MediaManager.h"
#include <QmlStatusBar/osdwriter.h> 
#include <QStringList>
#include <QApplication>

OSDManager * OSDManager::pOSDManager = NULL;

OSDManager::OSDManager(AppMediaPlayer_EngineMain * pEngineMain)
: OSDAbstractManager(pEngineMain){
	m_screenManager = new OSDScreenManager(pEngineMain);

	connectOSDsignalToSlot(); //added by edo.lee 2013.12.14 

	resetLastInfo();

	connect(m_screenManager, SIGNAL(auxModeEntered()), SLOT(displayAuxMode()));

	connect(m_screenManager, SIGNAL(clearAvOSD(unsigned int)), SLOT(clearAvOSD(unsigned int)));
	connect(m_screenManager, SIGNAL(agreeScreenShow()), SLOT(showCurrentOSD()));
	connect(m_screenManager, SIGNAL(lastIncomedOSD()), SLOT(showCurrentOSD()));
	connect(m_screenManager, SIGNAL(clearConnectInfo()), SLOT(clearConnectInfo()));
	


	connect( engine()->GetMediaManager(), SIGNAL(discLoading()), SLOT(onDiscLoading()));
	connect( engine()->GetMediaManager(), SIGNAL(deviceRemoved(MEDIA_SOURCE_T, MEDIA_TYPE_T)), SLOT(onDeviceRemoved(MEDIA_SOURCE_T, MEDIA_TYPE_T)));
}

OSDManager::~OSDManager(){ }

void OSDManager::setFileManagerScreen(DISPLAY_T display){
	m_screenManager->setFileManagerScreen(display);
}

void OSDManager::setFileManagerToBG(){
	m_screenManager->setFileManagerBG();
}

/*!
	\fn void OSDManager::displayOSD(OSD_DEVICE device, QString title, int shuffle, int repeat, int scan, bool isTune)

	Display OSD with title, playing mode and tune mode.
	If isTune is false, does not store arguments.
	If isTune is true, save arguments and use this arguments when update OSD(reset play time to intial value)

	This function filters current mode with device argument

	\sa OSDManager::updatePosition(), OSDManager::updateMetaData(), OSDManager::updatePlayingMode()
*/
void OSDManager::displayOSD(OSD_DEVICE device, QString title, int shuffle, int repeat, int scan, bool isTune){
	MP_LOG << "OnScreenDisplay device(" << device << ") : " << title << ", " << isTune << ", shuffle : " << shuffle << ", repeat : " << repeat << ", scan : " << scan << LOG_ENDL;
	if ( !compareDeviceToMode(device)){
                MP_HIGH << "OnScreenDisplay : Mode is not match with OSD" << LOG_ENDL;
		return;
	}
	else if ( title.isEmpty()){
            MP_LOG << "OnScreenDisplay : title empty" << LOG_ENDL;
			// modified by ravikanth on 28-09-13 for ITS 0191912
            if ( !isTune ){
                saveToLastInfo(device, title, shuffle, repeat, scan);
            }
            return;
	}

	bool showOSD = true;

	if ( !isTune ){
		saveToLastInfo(device, title, shuffle, repeat, scan);
	}

	m_lastInfo.m_isTune = isTune;
	
	// added by edo.lee 2013.11.05 ITS 0206260
	if ( !m_screenManager->agreeShow()){
		MP_LOG << "OnScreenDisplay : agree not showed" << LOG_ENDL;	
		showOSD = false;
	}
	//else 
	if ( m_screenManager->blockOSD()){
		MP_LOG << "OnScreenDisplay : blockOSD" << LOG_ENDL;
		showOSD = false;
	}

	QString prefix = createPrefix();
	if ( prefix.isEmpty()){
		MP_LOG << "OnScreenDisplay : prefix empty" << LOG_ENDL;
		return;
	}

	// already saved title to m_lastInfo.m_title
	QString strOSD = createContent((isTune?title:""));
	QString modeString = (isTune? "":getLastModeString());
	if ( !modeString.isEmpty()) modeString.prepend("^");

	if ( showOSD)
		OSDWriter::SendOSD(prefix + strOSD + modeString);

	if ( !isTune){
		resetLastAdditionalInfo();
		m_lastInfo.m_prefix = prefix;
	}
	MP_LOG << "OnScreenDisplay : " << prefix + strOSD + modeString << LOG_ENDL;
}

/*!
	\fn void OSDManager::showCurrentOSD(OSD_DEVICE device, int pos, bool isLong, bool isCurrentShow)
	
	Show OSD using stored last information in displayOSD()
	It will trigger 5 sec OSD again in case of FF/REW(isLong), Tune Enter(isCurrentShow) and positition is arrived at first.
	Other cases will only update OSD. 

	\sa OSDManager::displayOSD(), OSDManager::updatePosition(), OSDManager::updatePlayingMode(), OSDManager::updateMetaData()
*/
void OSDManager::showCurrentOSD(OSD_DEVICE device, int pos, bool isLong, bool isCurrentShow){
	if ( !compareDeviceToMode(device)){
		MP_LOG << "OnScreenDisplay : Mode is not match with OSD : currentMode(" << currentMode() << ", income device(" << device << ")" << LOG_ENDL;
		return;
	}

	if ( device == DEVICE_MAX || device != m_lastInfo.m_device) {
		MP_LOG << "OnScreenDisplay : device error(" << m_lastInfo.m_device << ") osd(" << device <<")" << LOG_ENDL;
		return;
	}
	
	m_lastInfo.m_pos = pos;

	if ( isLong )
		m_lastInfo.m_isTune = false;

	if ( m_lastInfo.m_isTune ) {
        //MP_LOG << "OnScreenDisplay : last is tune discarded" << LOG_ENDL; // removed by sangmin.seol 2014.06.27 reduce log for performance
		return;
	}
	else if ( m_screenManager->blockOSD()){
		MP_LOG << "OnScreenDisplay : blockOSD Update" << LOG_ENDL;
		return;
	}

	QString modeString = getLastModeString();
	QString timeString = getLastTimeString();

	if ( !modeString.isEmpty()){
		timeString.prepend("^");
		timeString.prepend(modeString);
		timeString.prepend("^");
	}
	else{
		timeString.prepend("^");
	}

	QString updateString;

	if ( isLong || isCurrentShow)
		updateString = "";
	else
		updateString = ":UPDATE";

	QString prefix;
	if ( isCurrentShow )
		prefix = createPrefix();
	else
		prefix = createPrefix(true);

	QString completeOSDString = prefix + updateString + createContent() + timeString;
	OSDWriter::SendOSD(completeOSDString);
    //MP_LOG<<"TanDebug "<<completeOSDString << LOG_ENDL; //removed too many logs...
}

/*!
	\fn OSDManager::updatePlayingMode(OSD_DEVICE device, int shuffle, int repeat, int scan)

	Update OSD with playing mode ( shuffle, repeat, scan )
	
	Update Playing Mode with stored information in displayOSD and other updateMethod		

	\sa OSDManager::displayOSD()
*/
void OSDManager::updatePlayingMode(OSD_DEVICE device, int shuffle, int repeat, int scan){
	if ( !compareDeviceToMode(device)){
		MP_LOG << "OnScreenDisplay : Mode is not match with OSD device : " << device << LOG_ENDL;
		return;
	}

	if ( device == DEVICE_MAX || device != m_lastInfo.m_device) return;

	m_lastInfo.m_shuffle = shuffle;
	m_lastInfo.m_repeat = repeat;
	m_lastInfo.m_scan = scan;

	if ( m_lastInfo.m_isTune ) return;
	
	QString modeString = getLastModeString();
	QString timeString = getLastTimeString();

	if ( !timeString.isEmpty()){
		modeString.prepend("^");
		modeString.prepend(timeString);
		modeString.prepend("^");
	}
	else{
		modeString.prepend("^");
	}

	QString completeOSDString = createPrefix() + ":UPDATE" + createContent() + modeString;
	OSDWriter::SendOSD(completeOSDString);
    MP_LOG << "TanDebug OnScreenDisplay Mode : " << completeOSDString << LOG_ENDL;
}

/*!
	\fn OSDManager::updateMeataData(OSD_DEVICE device, QString title)

	Update OSD with Title

	Update Title with stored information in displayOSD and other updateMethod
	
	\sa OSDManager::displayOSD()
*/
void OSDManager::updateMetaData(OSD_DEVICE device, QString title){
	MP_LOG << "OnScreenDisplay(" << title << ")" << LOG_ENDL;
	if ( !compareDeviceToMode(device)){
		MP_LOG << "OnScreenDisplay : Mode is not match with OSD device : " << device << LOG_ENDL;
		return;
	}
	m_lastInfo.m_title = title;	
	showCurrentOSD(device, m_lastInfo.m_pos, false, false);
}
// { added by wonseok.heo for ITS 197433 2013.10.28
void OSDManager::updateCDDAOSDMetaData(OSD_DEVICE device, QString title){
        MP_LOG << "updateCDDAOSDMetaData(" << title << ")" << LOG_ENDL;
        if ( !compareDeviceToMode(device)){
                MP_LOG << "updateCDDAOSDMetaData : Mode is not match with OSD device : " << device << LOG_ENDL;
                return;
        }
        m_lastInfo.m_title = title;
}// } added by wonseok.heo for ITS 197433 2013.10.28

/*!
	\fn OSDManager::displayDVDMenuOSD(bool bothScreen)

	Display OSD when DVD Video is in title menu or intro

	When bothScreen argument is true, it will display OSD front / rear, else ScreenManager will decide screen.
*/
void OSDManager::displayDVDMenuOSD(bool bothScreen){
    MP_LOG << "OnScreenDisplay : " << bothScreen << LOG_ENDL;
    // { modified by cychoi 2013.11.12 for OSD Title Translation
    if ( bothScreen ){
        QString info = QString("@OSD:FR^MEDIA^INFO^DVD^");
        info += qApp->translate("main", "STR_MEDIA_MNG_DVD");
        info += QString("^VIDEO^");
        info += qApp->translate("main", "STR_MEDIA_SELECT_DISK_MENU");
        OSDWriter::SendOSD(info);
    }
    else{
        displayOSD(DISC_DVD_VIDEO, "%STR_MEDIA_SELECT_DISK_MENU%", MP::RANDOMOFF, MP::REPEATOFF, MP::SCANOFF , false);
    }
    // } modified by cychoi 2013.11.12
}

/*!
	OSDManager::displayOSDWithDiscError()

	Display OSD with disc error.
*/
void OSDManager::displayDiscErrorOSD(int error, DISPLAY_T display){ // modified by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    MP_LOG << "OnScreenDisplay" << LOG_ENDL;
    // { modified by cychoi 2013.11.12 for OSD Title Translation
    // { modified by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    QString strOSD;
    if(display == DISPLAY_NONE)
        return;
    else if(display == DISPLAY_FRONT)
        strOSD = QString("@OSD:F^MEDIA_INFO^ERR^DISC^");
    else if(display == DISPLAY_REAR)
        strOSD = QString("@OSD:R^MEDIA_INFO^ERR^DISC^");
    else
        strOSD = QString("@OSD:FR^MEDIA_INFO^ERR^DISC^");
    // } modified by cychoi 2014.04.09
    strOSD += qApp->translate("main", ("STR_MEDIA_DISC"));
    strOSD += QString("^MULTI_LINE^"); // modified by cychoi 2014.02.16 for Multi-line OSD
    // } modified by cychoi 2013.11.12
    switch(error)
    {
    case PLAYER_DISCCORRUPTED:
    case PLAYER_DISC_READ_ERROR:
    case PLAYER_NOMEDIA:
    case PLAYER_FILENOTFOUND: // added by cychoi 2014.05.22 for Deck is not connected
    {		      
        strOSD += qApp->translate("main","STR_OSD_NOT_SUPPORTED_DISC");
        break;
    }

    case PLAYER_HIGHTEMPERATURE:
    {          
        strOSD += qApp->translate("main","STR_MEDIA_STOP_FUNCTION");
        break;
    }

    case PLAYER_DISC_REGION_ERROR:
    {              
        strOSD += qApp->translate("main","STR_MEDIA_LOCAL_ERROR");
        break;
    }

    default: //make compiler happy
        break;
    }
    OSDWriter::SendOSD(strOSD);
}

/*!
	\fn OSDManager::showCurrentOSD()

	Display OSD with last stored information
*/
void OSDManager::showCurrentOSD(){
	MP_LOG << "OnScreenDisplay" << LOG_ENDL;
	if ( compareDeviceToMode(m_lastInfo.m_device)){
		if ( m_lastInfo.m_pos == -1)
			displayOSD(m_lastInfo.m_device, m_lastInfo.m_title, m_lastInfo.m_shuffle, m_lastInfo.m_repeat, m_lastInfo.m_scan, false);
		else
			displayOSDWithTime();
	}
}

/*!
	\fn OSDManager::updatePosition()

	Update OSD with incomed pos

	When isLong(FF/REW) is true, it will trigger OSD show.
	When pos is first arrived after displayOSD, it will also trigger OSD show, other cases will do only update

	\sa OSDManager::showCurrentOSD(OSD_DEVICE device, int pos, bool isLong, bool isCurrentShow)
*/
void OSDManager::updatePosition(OSD_DEVICE device, int pos, bool isLong){
	// MP_LOG << "OnScreenDisplay : device : " << device << ", pos : " << pos << LOG_ENDL;
	showCurrentOSD(device, pos, isLong, false);	
}

/*!
	OSDManager::displayOSDWithTime()

	Display OSD with time(position).

	It is different with OSDManager::showCurrentOSD() turn off last saved tune option to normal.

	\sa OSDManager::showCurrentOSD(OSD_DEVICE device, int pos, bool isLong, bool isCurrentShow)
*/
void OSDManager::displayOSDWithTime(){
	MP_LOG << "OnScreenDisplay" << LOG_ENDL;
	m_lastInfo.m_isTune = false;
	showCurrentOSD(m_lastInfo.m_device, m_lastInfo.m_pos, false, true);	
}

/*!
	\fn OSDManager::onDiscLoading()

	Show OSD when Disc is loading on both screen.
*/
void OSDManager::onDiscLoading(){
	MP_LOG << "OnScreenDisplay" << LOG_ENDL;
    // { modified by cychoi 2013.11.12 for OSD Title Translation
    QString strOSD = QString("@OSD:FR^CONNECT_INFO^LOADING^DISC^");
    strOSD += qApp->translate("main", ("STR_MEDIA_DISC"));
    strOSD += QString("^");
    // } modified by cychoi 2013.11.12
	strOSD += qApp->translate("main", "STR_READ_DISC_INFO");
	OSDWriter::SendOSD(strOSD);
}

/*
	\fn OSDManager::onDeviceRemoved()

	Handle AUX remove events and show OSD.
*/
void OSDManager::onDeviceRemoved(MEDIA_SOURCE_T source, MEDIA_TYPE_T){
	MP_LOG << "OnScreenDisplay" << LOG_ENDL;
	if ( source == MEDIA_SOURCE_AUX1 || source == MEDIA_SOURCE_AUX2){
		displayOSD(OSD_EVENT_AUX_DISCONNECTED);
	}
}

void OSDManager::displayDVDMenuMode(){
	displayDVDMenuOSD(false);
}

void OSDManager::displayAuxMode(){
	MP_LOG << "OnScreenDisplay" << LOG_ENDL;
	if ( currentMode() == MODE_STATE_AUX1_AUDIO || currentMode() == MODE_STATE_AUX2_AUDIO){
		displayOSD(OSD_EVENT_AUX_EXTERNAL_DEVICE_CONNECTED ,eAPP_AVP_AUDIO_MODE_AUX );//modified by yongkyun.lee 2013-12-10 for : ITS 212466
	}
	else if ( currentMode() == MODE_STATE_AUX1_VIDEO || currentMode() == MODE_STATE_AUX2_VIDEO){
		displayOSD(OSD_EVENT_AUX_EXTERNAL_DEVICE_CONNECTED ,eAPP_AVP_VIDEO_MODE_AUX );//modified by yongkyun.lee 2013-12-10 for : ITS 212466
	} 
}

void OSDManager::clearAvOSD(unsigned int aDisplay){
	DISPLAY_T display = (DISPLAY_T) aDisplay;
	if ( display == DISPLAY_FRONT){
		MP_LOG << "OnScreenDisplay : Front CLEAR" << LOG_ENDL;
		OSDWriter::SendOSD("@OSD:F:CLEAR_AV");	
	}
	else if ( display == DISPLAY_REAR){
		MP_LOG << "OnScreenDisplay : Rear CLEAR" << LOG_ENDL;
		OSDWriter::SendOSD("@OSD:R:CLEAR_AV");
	}
	else if ( display == DISPLAY_FRONT_REAR){
		MP_LOG << "OnScreenDisplay : Front/Rear CLEAR" << LOG_ENDL;
		OSDWriter::SendOSD("@OSD:FR:CLEAR_AV");
	}
}

void OSDManager::clearConnectInfo(){
	QString clearStr = "@OSD:";
	if ( OSDMode::IsAuxMode(currentMode())){
		if ( OSDEntry::IsAuxEntry(currentFrontEntry())) clearStr += "F";
		if ( OSDEntry::IsAuxEntry(currentRearEntry())) clearStr += "R";

		OSDWriter::SendOSD(clearStr + ":CLEAR^CONNECT_INFO^WARN^AUX");
	}
	else if ( OSDMode::IsUsbMode(currentMode())){
		if ( OSDEntry::IsUsbEntry(currentFrontEntry())) clearStr += "F";
		if ( OSDEntry::IsUsbEntry(currentRearEntry())) clearStr += "R";

		OSDWriter::SendOSD(clearStr + ":CLEAR^CONNECT_INFO^LOADING^USB");
	}
	else if ( OSDMode::IsDiscMode(currentMode())){
		if ( OSDEntry::IsDiscEntry(currentFrontEntry())) clearStr += "F";
		if ( OSDEntry::IsDiscEntry(currentRearEntry())) clearStr += "R";

		OSDWriter::SendOSD(clearStr + ":CLEAR^CONNECT_INFO^LOADING^DISC");
	}
	else if ( OSDMode::IsIpodMode(currentMode())){
		if ( OSDEntry::IsIpodEntry(currentFrontEntry())) clearStr += "F";
		if ( OSDEntry::IsIpodEntry(currentRearEntry())) clearStr += "R";

		OSDWriter::SendOSD(clearStr + ":CLEAR^CONNECT_INFO^LOADING^IPOD");
	}
}

void OSDManager::displayNoMediaFileOSD(MEDIA_SOURCE_T source){
    QString strOSD = "@OSD:FR^";
    if ( source == MEDIA_SOURCE_IPOD1 || source == MEDIA_SOURCE_IPOD2)
    {   
        strOSD += "MEDIA_INFO^WARN^IPOD^iPod^";
    }
    // { modified by cychoi 2013.11.12 for OSD Title Translation
    else if ( source == MEDIA_SOURCE_USB1 || source == MEDIA_SOURCE_USB2)
    {   
        strOSD += QString("MEDIA_INFO^WARN^USB^");
        strOSD += qApp->translate("main", ("STR_MEDIA_USB"));
        strOSD += QString("^");
    }
    else
    {   
        strOSD += QString("MEDIA_INFO^WARN^JUKEBOX^");
        strOSD += qApp->translate("main", ("STR_MEDIA_JUKEBOX"));
        strOSD += QString("^");
    }
    // }modified by cychoi 2013.11.12

    strOSD += qApp->translate("main", "STR_NO_PLAYABLE_FILE");
    OSDWriter::SendOSD(strOSD);
}



void OSDManager::resetLastInfo(){
	m_lastInfo.m_device = DEVICE_MAX;
	m_lastInfo.m_isTune = false;
	m_lastInfo.m_shuffle = MP::RANDOMOFF;
	m_lastInfo.m_scan = MP::SCANOFF;
	m_lastInfo.m_repeat = MP::REPEATOFF;
	resetLastAdditionalInfo();
}

void OSDManager::resetLastAdditionalInfo(){
	m_lastInfo.m_pos = -1;
}

void OSDManager::saveToLastInfo(OSD_DEVICE device, const QString & title, int shuffle, int repeat, int scan){
	m_lastInfo.m_device = device;
	m_lastInfo.m_title = title;
	m_lastInfo.m_shuffle = shuffle;
	m_lastInfo.m_repeat = repeat;
	m_lastInfo.m_scan = scan;
}

void OSDManager::preHandleUishEvent(Event aEvent, DISPLAY_T display){
	m_screenManager->preHandleUishEvent(aEvent, display);
}

void OSDManager::postHandleUishEvent(Event aEvent, DISPLAY_T display){
	m_screenManager->postHandleUishEvent(aEvent, display);
}

QString OSDManager::translateTitle(const QString & str){
	if ( str.startsWith("%") && str.endsWith("%") && str.length() > 1){
		if ( str == "%STR_MEDIA_SELECT_DISK_MENU%")
			return qApp->translate("main", "STR_MEDIA_SELECT_DISK_MENU");
		else if ( str == "%STR_EXTERNAL_DEVICE_CONNECTED%")
			return qApp->translate("main", "STR_EXTERNAL_DEVICE_CONNECTED");
	}
	return str;
}

void OSDManager::displayOSD(unsigned int nEvent, APP_ENTRY_POINT_T entry){
    MP_LOG << "OnScreenDisplay : " << nEvent << ", entry : " << entry << LOG_ENDL;
    QString strOSD = "@OSD:";
    QString info;
    DISPLAY_T outputScreen = outputEvent(nEvent, entry);

    // TODO: get device string from entry, not fixed string!!! and caller should pass entry argument
    if ( outputScreen == DISPLAY_NONE) return;
    else if ( outputScreen == DISPLAY_FRONT) strOSD.append("F");
    else if ( outputScreen == DISPLAY_REAR) strOSD.append("R");
    else if ( outputScreen == DISPLAY_FRONT_REAR) strOSD.append("FR");

    strOSD.append("^");

    switch(nEvent){
    // { modified by cychoi 2013.11.12 for OSD Title Translation
    case OSD_EVENT_AUX_EXTERNAL_DEVICE_CONNECTED:
        //{ modified by yongkyun.lee 2013-12-10 for : ITS 212466 , Changed icon
        info = QString("CONNECT_INFO^WARN^AUX^");
        info += qApp->translate("main", ("STR_MEDIA_AUX"));
        info += QString("^");
        info += qApp->translate("main", "STR_EXTERNAL_DEVICE_CONNECTED");
        //} modified by yongkyun.lee 2013-12-10 
        break;
    case OSD_EVENT_UNSUPPORTED_ACTION:
        info = QString("MEDIA_INFO^ERR^AUX^");
        info += qApp->translate("main", ("STR_MEDIA_AUX"));
        info += QString("^");
        info += qApp->translate("main", "STR_OPERATION_NOT_SUPPORTED");
        break;
    // }modified by cychoi 2013.11.12
    case OSD_EVENT_IPOD_SENDINGTAGS:
        info += "MEDIA_INFO^WARN^IPOD^iPod^";	
        info += qApp->translate("main", "STR_IPOD_TAGGING_SENDING_TAGS_TO_IPOD");
        break;
    case OSD_EVENT_IPOD_MEMORYFULL:
        info += "MEDIA_INFO^WARN^IPOD^iPod^";
        info += qApp->translate("main", "STR_IPOD_TAGGING_MEMORY_FULL");
        break;
    case OSD_EVENT_IPOD_SUCCESS:
        info += "MEDIA_INFO^WARN^IPOD^iPod^";
        info += qApp->translate("main", "STR_IPOD_TAGGING_TAGGING_SUCCESSFUL");
        break;
    case OSD_EVENT_IPOD_FAILED:
        info += "MEDIA_INFO^WARN^IPOD^iPod^";
        info += qApp->translate("main", "STR_IPOD_TAGGING_TAGGING_FAILED");
        break;
    case OSD_EVENT_IPOD_UNSUPPORTED:
        info += "MEDIA_INFO^ERR^IPOD^iPod^";
        info += qApp->translate("main", "STR_IPOD_UNSUPPORTED");
        break;
    case OSD_EVENT_IPOD_CONNECTION_ERROR:
        info += "MEDIA_INFO^ERR^IPOD^iPod^";
        info += qApp->translate("main", "STR_IPOD_CONNECTION_ERROR");
        break;
    case OSD_EVENT_IPOD_READING:
        // Modified by jonghwan.cho@lge.com 2013.12.09.
        info += "CONNECT_INFO^LOADING^IPOD^%STR_IPOD%^%STR_IPOD_READING%";
        //info += qApp->translate("main", "STR_IPOD_READING");
        break;
    // { modified by cychoi 2013.11.12 for OSD Title Translation
    case OSD_EVENT_COPY_COMPLETED:
        info = QString("MEDIA_INFO^WARN^JUKEBOX^");
        //if(entry == eAPP_AVP_AUDIO_MODE_JUKEBOX)
        //    info += qApp->translate("main", ("STR_MEDIA_AUDIO_JUKEBOX"));
        //else
        //    info += qApp->translate("main", ("STR_MEDIA_VIDEO_JUKEBOX"));
        //info += QString("^");
        info += QString("JUKEBOX^");
        info += qApp->translate("main", "STR_MEDIA_MNG_COPY_COMPLETED");
        break;
    case OSD_EVENT_DELETE_COMPLETED:
        info = QString("MEDIA_INFO^WARN^JUKEBOX^");
        if(entry == eAPP_AVP_AUDIO_MODE_JUKEBOX)
            info += qApp->translate("main", ("STR_MEDIA_AUDIO_JUKEBOX"));
        else
            info += qApp->translate("main", ("STR_MEDIA_VIDEO_JUKEBOX"));
        info += QString("^");
        info += qApp->translate("main", "STR_MEDIA_MNG_DELETE_COMPLETED");
        break;
    case OSD_EVENT_MOVE_COMPLETED:
        info = QString("MEDIA_INFO^WARN^JUKEBOX^");
        if(entry == eAPP_AVP_AUDIO_MODE_JUKEBOX)
            info += qApp->translate("main", ("STR_MEDIA_AUDIO_JUKEBOX"));
        else
            info += qApp->translate("main", ("STR_MEDIA_VIDEO_JUKEBOX"));
        info += QString("^");
        info += qApp->translate("main", "STR_OSD_MOVING_COMPLITED");
        break;
    case OSD_EVENT_ADISC_EJECTED:
        info = QString("CONNECT_INFO^WARN^DISC^");
        info += qApp->translate("main", ("STR_MEDIA_DISC"));
        info += QString("^");
        info += qApp->translate("main", "STR_OSD_EJECTED_DISC");
        break;
    case OSD_EVENT_NO_INSERTED_DISC:
        info = QString("CONNECT_INFO^WARN^DISC^");
        info += qApp->translate("main", ("STR_MEDIA_DISC"));
        info += QString("^");
        info += qApp->translate("main", "STR_OSD_NO_INSERTED_DISC");
        break;
    case OSD_EVENT_AUX_DISCONNECTED:
        info = QString("CONNECT_INFO^WARN^AUX^");
        info += qApp->translate("main", ("STR_MEDIA_AUX"));
        info += QString("^");
        info += qApp->translate("main", "STR_AUX_DISCONNECTED");
        break;
/*
    case OSD_EVENT_AUX_CONNECTED:
        info += QString("CONNECT_INFO^WARN^AUX^");
        info += qApp->translate("main", ("STR_MEDIA_AUX"));
        info += QString("^");
        info += qApp->translate("main", "STR_AUX_CONNECTED");
        break;
*/
    case OSD_EVENT_BT_UNSUPPORTED:
        info = QString("MEDIA_INFO^ERR^BLUETOOTH^");
        info += qApp->translate("main", ("STR_MEDIA_BLUETOOTH"));
        info += QString("^");
        info += qApp->translate("main", "STR_OPERATION_NOT_SUPPORTED");
        break;
    // }modified by cychoi 2013.11.12

    //{added by junam 2013.08.27
    case OSD_EVENT_IPOD_UNSUPPORTED_ACTION:
    case OSD_EVENT_IPOD_UNSUPPORTED_ACTION_FG:
        info += "MEDIA_INFO^ERR^IPOD^iPod^";
        info += qApp->translate("main", "STR_OPERATION_NOT_SUPPORTED");
        break;
    //}added by junam

    case OSD_EVENT_IPOD_EMPTY_TITLE:
        info += "MEDIA^INFO^IPOD^iPod^MUSIC^";
        info += qApp->translate("main", "STR_MEDIA_UNKNOWN");
        break;

    default: break;
    }

    if ( !info.isEmpty()) OSDWriter::SendOSD(strOSD + info);
}

inline QString OSDManager::createContent(const QString & aTitle){
	QString title = aTitle;
	QString strOSD = "^MEDIA^";
	if ( m_lastInfo.m_isTune ) strOSD.append("INFO_DIAL^");
	else strOSD.append("INFO^");
	
	QString deviceString = getDeviceString(m_lastInfo.m_device);
	if ( deviceString.isEmpty()) {
		MP_LOG << "OnScreenDisplay : Device String Empty" << LOG_ENDL;
		return QString();
	}

	strOSD.append(deviceString);

	if ( aTitle.isEmpty())
		title = translateTitle(m_lastInfo.m_title);
	else
		title = aTitle;

	title.replace("^", "\\^");
	title.replace("@", "\\@");
	strOSD.append(title);

	return strOSD;
}

inline QString OSDManager::createPrefix(bool isLong){
	QString prefix = "@OSD:";
	if ( getDisplayString(isLong).isEmpty()) {
		MP_LOG << "OnScreenDisplay : Display String Empty" << LOG_ENDL;
		return QString();
	}

	prefix.append(getDisplayString(isLong));
	return prefix;
}

inline QString OSDManager::getDisplayString(bool isLong){
	DISPLAY_T disp = m_screenManager->displayOfOSD(isLong);
	if ( disp == DISPLAY_FRONT)
		return "F";
	else if ( disp == DISPLAY_REAR)
		return "R";
	else if ( disp == DISPLAY_FRONT_REAR)
		return "FR";

	return QString();
}

inline QString OSDManager::getDeviceString(OSD_DEVICE device){
    // { modified by cychoi 2013.11.12 for OSD Title Translation
    QString info;
    switch(device){
    case JUKEBOX_MUSIC:
        info = QString("JUKEBOX^");
        info += qApp->translate("main", ("STR_MEDIA_AUDIO_JUKEBOX"));
        info += QString("^MUSIC^");
        return info;
    case JUKEBOX_VIDEO: 
        info = QString("JUKEBOX^");
        info += qApp->translate("main", ("STR_MEDIA_VIDEO_JUKEBOX"));
        info += QString("^VIDEO^");
        return info;
    case AUX_MUSIC: 
        info = QString("AUX^");
        info += qApp->translate("main", ("STR_MEDIA_AUX"));
        info += QString("^MUSIC^");
        return info;
    case AUX_VIDEO: 
        info = QString("AUX^");
        info += qApp->translate("main", ("STR_MEDIA_AUX"));
        info += QString("^VIDEO^");
        return info;
    case DISC_CDDA_MUSIC: 
        info = QString("DISC^");
        info += qApp->translate("main", ("STR_MEDIA_DISC_CD"));
        info += QString("^MUSIC^");
        return info;
    case DISC_MP3_MUSIC: 
        info = QString("DISC^");
        info += qApp->translate("main", ("STR_MEDIA_DISC_MP3"));
        info += QString("^MUSIC^");
        return info;
    case DISC_DVD_VIDEO: 
        info = QString("DVD^");
        info += qApp->translate("main", ("STR_MEDIA_MNG_DVD"));
        info += QString("^VIDEO^");
        return info;
    case DISC_VCD_VIDEO: 
        info = QString("VCD^");
        info += qApp->translate("main", ("STR_MEDIA_MNG_VCD"));
        info += QString("^VIDEO^");
        return info;
    case IPOD_MUSIC: 
        return QString("IPOD^iPod^MUSIC^");
    case USB_MUSIC: 
        info = QString("USB^");
        info += qApp->translate("main", ("STR_MEDIA_AUDIO_USB"));
        info += QString("^MUSIC^");
        return info;
    case USB_VIDEO: 
        info = QString("USB^");
        info += qApp->translate("main", ("STR_MEDIA_VIDEO_USB"));
        info += QString("^VIDEO^");
        return info;
    case BLUETOOTH_MUSIC: 
        info = QString("BLUETOOTH^");
        info += qApp->translate("main", ("STR_MEDIA_BLUETOOTH"));
        info += QString("^MUSIC^");
        return info;
    default: break;
    }
    return QString();
    // } modified by cychoi 2013.11.12
}

QString OSDManager::getLastModeString(){
	QStringList modeList;
	QString modeString;
    if ( m_lastInfo.m_shuffle != MP::RANDOMOFF)
    {
        modeList.append("(SH)");
    }
    else
    {
        if ( m_lastInfo.m_scan == MP::SCANALL )
            modeList.append("(SE)");
        else if ( m_lastInfo.m_scan == MP::SCANFILE)
            modeList.append("(SF)");
    }
    if ( m_lastInfo.m_repeat == MP::REPEATFILE)
        modeList.append("(RP1)");
    else if ( m_lastInfo.m_repeat == MP::REPEATFOLDER)
        modeList.append("(RPF)");
    else if ( m_lastInfo.m_repeat == MP::REPEATALL)
        modeList.append("(RPA)");

	for (int i = 0; i < modeList.count(); ++i){
		if ( i != 0 ) modeString.append("^");
		modeString.append(modeList.at(i));
	}
	return modeString;
}

QString OSDManager::getLastTimeString(){
	QString timeString; 

	if ( m_lastInfo.m_pos != -1)
		timeString.append(convertToSecond(m_lastInfo.m_pos, !isVideoModeCurrent()));
	return timeString;
}

inline QString OSDManager::convertToSecond(int duration, bool isAudio){
	QTime time;
	time = time.addMSecs(duration);
    if ( isAudio )
    {
        // Addded by jonghwan.cho@lge.com 2013.11.12. for ITS 0209346
        if( duration > 3600000 /*over 1 hour*/) return time.toString("(hh:mm:ss)");
        else time.toString("(mm:ss)");
    }
	return time.toString("(hh:mm:ss)");
}


//{ modified by yongkyun.lee 2013-09-09 for : AUX video->audio OSD
void OSDManager::setAUXDisconnect(bool val)
{
    m_screenManager->setAuxDisconnect(val);
}

//} modified by yongkyun.lee 2013-09-09 

//{ modified by yongkyun.lee 2013-10-07 for :  ITS 193970
bool OSDManager::getAUXDisconnect()
{
    return m_screenManager->getAuxDisconnect();
}
//} modified by yongkyun.lee 2013-10-07 

//{ added by hyochang.ryu 20131024 for ITS 192354
void OSDManager::checkAuxMode()
{
    m_screenManager->checkAuxMode();
}
//} added by hyochang.ryu 20131024 for ITS 192354
// added by suilyou 20131213 ITS 0214646 START
void OSDManager::connectOSDsignalToSlot()
{
    MP_LOG << LOG_ENDL;
    for (int i = 0; i < APP_MEDIA_MODE_MAX; ++i){
        connect( engine()->GetEngine((APP_MEDIA_MODE_T) i) , SIGNAL(setOSDMetaDataChanged(OSD_DEVICE, QString, int, int, int, bool)), SLOT(displayOSD(OSD_DEVICE, QString, int, int, int, bool)));
        connect( engine()->GetEngine( (APP_MEDIA_MODE_T) i) , SIGNAL(updatePositionChanged(OSD_DEVICE, int, bool)), SLOT(updatePosition(OSD_DEVICE, int, bool)));
        connect( engine()->GetEngine( (APP_MEDIA_MODE_T) i) , SIGNAL(updatePlayingModeChanged(OSD_DEVICE, int, int, int)), SLOT(updatePlayingMode(OSD_DEVICE, int, int, int)));
        connect( engine()->GetEngine( (APP_MEDIA_MODE_T) i) , SIGNAL(showCurrentOSDChanged()), SLOT(showCurrentOSD()));
        connect( engine()->GetEngine( (APP_MEDIA_MODE_T) i) , SIGNAL(updateOSDMetaDataChanged(OSD_DEVICE, QString)), SLOT(updateMetaData(OSD_DEVICE, QString)));
        connect( engine()->GetEngine( (APP_MEDIA_MODE_T) i) , SIGNAL(updateCDDAOSDMetaDataChanged(OSD_DEVICE, QString)), SLOT(updateCDDAOSDMetaData(OSD_DEVICE,QString))); // added by wonseok.heo for ITS 197433 2013.10.28
    }
}

// added by suilyou 20131213 ITS 0214646 END
