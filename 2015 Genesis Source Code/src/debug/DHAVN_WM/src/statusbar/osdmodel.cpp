#include "osdmodel.h"
#include "common.h"
#include "osdview.h"
#include "dhavnlog.h"

OSDModel::OSDModel(bool isFront, QObject * obj)
: QObject(obj){
	m_view = NULL;
	m_activate = false;
	m_front = isFront;

	m_currentOSD = NULL;

	// AV List declaration
//	m_avScreenList.append("APPMEDIAPLAYER");
	m_avScreenList.append("APPAHARADIO");
//	m_avScreenList.append("APPINTERACTIVE");
//	m_avScreenList.append("APPMOBILETV");
	m_avScreenList.append("APPRADIO");
	m_avScreenList.append("APPVR");
	m_avScreenList.append("APPDAB");
	m_avScreenList.append("APPXMAUDIO");
	m_avScreenList.append("APPPANDORA");

	m_avCommandList.append("RADIO");
	m_avCommandList.append("SXM");
	m_avCommandList.append("HDRADIO");
	m_avCommandList.append("DAB");
	m_avCommandList.append("DAB_INFO");
	m_avCommandList.append("RDS");
	m_avCommandList.append("DMB");
	m_avCommandList.append("MEDIA_INFO");
	m_avCommandList.append("MEDIA_INFO_MODE");
	m_avCommandList.append("MEDIA");
	m_avCommandList.append("AHA");
        m_avCommandList.append("EMERGENCY"); //dmchoi

        m_avCommandClearList.append("RADIO");
        m_avCommandClearList.append("SXM");
        m_avCommandClearList.append("HDRADIO");
        m_avCommandClearList.append("DAB");
        m_avCommandClearList.append("DAB_INFO");
        m_avCommandClearList.append("RDS");
        m_avCommandClearList.append("MEDIA_INFO");
        m_avCommandClearList.append("MEDIA_INFO_MODE");
        m_avCommandClearList.append("MEDIA");
        m_avCommandClearList.append("AHA");
        m_avCommandClearList.append("EMERGENCY"); //dmchoi

	// Bluetooth List declaration
	m_bluetoothScreenList.append("APPBTPHONE");
	m_bluetoothCommandList.append("BLUETOOTH");

	// Info List declaration
	m_infoScreenList.append("APPINFO");
	m_infoCommandList.append("CLIMATE");
//	m_infoCommandList.append("DRIVE");  // DRIVE OSD 는 공조와 별개이므로, 공조 Full 화면에서 사라지게 하지 않는다.(ITS 0205313)
	m_infoCommandList.append("SEAT_CONTROL");
}

void OSDModel::setView(OSDView * v){
	m_view = v;
}

void OSDModel::lockOSD(bool isFront, bool isActivate){
	if ( isFront && m_front ){
		m_activate = isActivate;

		if ( !m_activate )
			clearAll();
	}
	else if ( !isFront && !m_front ){
		m_activate = isActivate;
		if ( !m_activate)
			clearAll();
	}
}

void OSDModel::clearAll(){
        lprintf("clearAll");
	while( !m_dataStack.isEmpty())
		delete m_dataStack.takeFirst();

	m_view->hide();	
}

int OSDModel::getIndexById(uchar id){
	if ( id == 0 ) return -1;

	for (int i = 0; i < m_dataStack.count(); ++i){
		if ( m_dataStack.at(i)->getId() == id)
			return i;
	}
	return -1;
}

bool OSDModel::checkShowOSD(int d){
	if ( !m_activate)
		return false;

	if ( (WM_REAR_VAL & d) == 0 && !m_front )	
		return false;

	if ( (WM_FRONT_VAL & d) == 0 && m_front )
		return false;

	return true;
}

void OSDModel::processNormalOSD(const QStringList & aCommand, uchar id){
	OSDData * data = new OSDData(id, OSDData::Normal, aCommand, this);
	connect(data, SIGNAL(timeout()), SLOT(osdTimeout()));
	deleteNormalOSDs();
	m_dataStack.append(data);

	data->startOSD();
}

void OSDModel::processDuplicateOSD(OSDData * data, bool isIncoming){
	deleteNormalOSDs();
	
	if ( isIncoming ){
		m_dataStack.removeAll(data);
		m_dataStack.append(data);	
	}
	else{
		int bluetoothIndex = getBluetoothStayIndex();
		int bluelinkIndex = getBluelinkStayIndex();
		int minIndex = -1;

		m_dataStack.removeAll(data);

		if ( bluelinkIndex > -1 || bluetoothIndex > -1){
			if ( bluelinkIndex == -1)
				minIndex = bluetoothIndex;
			else if ( bluetoothIndex == -1)
				minIndex = bluelinkIndex;
			else
				minIndex = qMin(bluetoothIndex, bluelinkIndex);

			m_dataStack.insert(minIndex, data);
		}
		else{
			m_dataStack.append(data);
		}
	}
}


void OSDModel::processStayOSD(const QStringList & aCommand, uchar id){
	QString command = aCommand.at(0);
	bool isIncomingCall = command.startsWith("BLUETOOTH") || (command.count() > 1 && command.startsWith("IBOX") && aCommand.at(1) == "BLUELINK");
	OSDData * data = NULL;

	if ( (data = isStackContainStay(command)) != NULL){
		lprintf("Duplicate STAY OSD : %s", qPrintable(command));
		processDuplicateOSD(data, isIncomingCall);
		data->changeData(aCommand);
		if ( m_currentOSD && m_currentOSD == data){
			m_view->displayOSD(data);
		}

		return;
	}

	
	data = new OSDData(id, OSDData::Stay, aCommand, this);
	connect(data, SIGNAL(timeout()), SLOT(osdTimeout()));

	if ( isIncomingCall ){
		deleteNormalOSDs();
		m_dataStack.append(data);
	}
	else {	// not incoming call
		int bluetoothIndex = getBluetoothStayIndex();
		int bluelinkIndex = getBluelinkStayIndex();
		int minIndex = -1;

		if ( bluelinkIndex > -1 || bluetoothIndex > -1){
			if ( bluelinkIndex == -1)
				minIndex = bluetoothIndex;
			else if ( bluetoothIndex == -1)
				minIndex = bluelinkIndex;
			else
				minIndex = qMin(bluetoothIndex, bluelinkIndex);

			m_dataStack.insert(minIndex, data);
		}
		else{
                        deleteNormalOSDs();
			m_dataStack.append(data);
		}
	}
}

void OSDModel::processOSD(int d, const QString & opt, const QStringList & commands, uchar id){
	QString option = opt;
	int prevCount = m_dataStack.count();
	int idIndex = getIndexById(id);

	if ( option.isEmpty() && commands.isEmpty()) return;
	else if ( commands.count() > 0 && commands.at(0).isEmpty()) return;

        if(commands.isEmpty() == 0) {
             if(commands.contains("IBOX")) {
                 idIndex = -1;
             }
        }

	if ( idIndex == -1 && !checkShowOSD(d))
		return;

	if ( idIndex != -1)
		option = "UPDATE";

	// for backward compatibility
	if ( option.isEmpty() && (commands.contains("INFO_TIME") || commands.contains("INFO_DIAL_TIME"))){
		option = "UPDATE";
	}

	if ( option.isEmpty() || option == "NORMAL"){
		processNormalOSD(commands, id);
	}
	else if ( option == "STAY"){
		processStayOSD(commands, id);
	}
	else if ( option == "STAY_TIMEOUT"){
		int index = getIndexStack(commands.at(0));
		if ( index > -1){
			OSDData * data = m_dataStack.at(index);
			if ( data->getDataType() == OSDData::Stay){
				data->changeType(OSDData::StayTimeout);
				data->changeData(commands);

				if ( m_currentOSD && m_currentOSD == data)
					m_view->displayOSD(data);
			}
		}
		return;
	}
	else if ( option == "UPDATE"){
		int index = getIndexStack(commands.at(0));
		if ( index > -1){
			OSDData * data = m_dataStack.at(index);
			data->changeData(commands);

			if ( m_currentOSD && m_currentOSD == data)
				m_view->displayOSD(data);
		}
		return;
	}
	else if ( option == "CLEAR_AV"){
		clearAV();	
	}
	else if ( option == "CLEAR"){
		lprintf("CLEAR : %d", commands.count());
		if ( commands.count() == 0){
			lprintf("CLEAR ALL");
			while (!m_dataStack.isEmpty())
				delete m_dataStack.takeFirst();
		}
		else{
			clearCommands(commands);
		}
	}	

	processOSD(prevCount);
}

void OSDModel::processOSD(int prevCount){
//	lprintf("processOSD prevCount : %d", prevCount);
	if ( m_dataStack.count() == 0){
		m_currentOSD = NULL;

		m_view->hideOSD();

		if ( prevCount > 0){
			emit osdEmpty();
		}
	}
	else if ( m_dataStack.last() != m_currentOSD){
		m_currentOSD = m_dataStack.last();
		m_view->displayOSD(m_dataStack.last());
	}
}

void OSDModel::clearById(uchar id){
	int prevCount = m_dataStack.count();
	bool found = false;
	for (int i = 0; i < m_dataStack.count(); ++i){
		if ( m_dataStack.at(i)->getId() == id){
			found = true;
			OSDData * data = m_dataStack.takeAt(i);
			delete data;
			break;
		}
	}

	if ( found ){
		processOSD(prevCount);
	}
}

void OSDModel::clearCommands(const QStringList & cmd){
	for (int i = 0; i < m_dataStack.count(); ++i){
		bool findClear = true;

		if ( cmd.count() <= m_dataStack.at(i)->getCommand().count()){
			for (int j = 0; j < cmd.count(); ++j){
				if ( cmd.at(j) != m_dataStack.at(i)->getCommand().at(j)){
					findClear = false;
					break;
				}
			}
		}
		else{
			findClear = false;
		}

		if ( findClear ){
			OSDData * data = m_dataStack.takeAt(i);
			delete data;
			--i;
		}
	}
}

void OSDModel::deleteNormalOSDs(){
	for (int i = 0; i < m_dataStack.count(); ++i){
		if ( m_dataStack.at(i)->getDataType() == OSDData::Normal){
			OSDData * data = m_dataStack.takeAt(i);
			delete data;
			--i;
		}
	}
}

int OSDModel::getIndexStack(const QString & cmd){
	for (int i = 0; i < m_dataStack.count(); ++i){
		if ( m_dataStack.at(i)->getCommand().at(0) == cmd)
			return i;
	}
	return -1;
}

int OSDModel::getBluetoothStayIndex(){
	for (int i = 0; i < m_dataStack.count(); ++i){
		if ( m_dataStack.at(i)->getCommand().at(0) == "BLUETOOTH" && m_dataStack.at(i)->getDataType() == OSDData::Stay)
			return i;
	}
	return -1;

}

int OSDModel::getBluelinkStayIndex(){
	for (int i = 0; i < m_dataStack.count(); ++i){
		if ( m_dataStack.at(i)->getCommand().at(0) == "IBOX" && m_dataStack.at(i)->getCommand().count() > 1 &&
				m_dataStack.at(i)->getCommand().at(1) == "BLUELINK" && m_dataStack.at(i)->getDataType() == OSDData::Stay)
			return i;
	}
	return -1;
}

OSDData * OSDModel::isStackContainStay(const QString & cmd){
	for (int i = 0; i < m_dataStack.count(); ++i){
		if ( m_dataStack.at(i)->getCommand().at(0) == cmd && m_dataStack.at(i)->getDataType() == OSDData::Stay)
			return m_dataStack[i];
	}
	return NULL;
}

void OSDModel::checkScreen(bool isFront, const QString & aName){
	lprintf("OSDModel: checkScreen : %s", qPrintable(aName));
	if ( !m_activate || aName.isNull() || aName.isEmpty()){
		return;
	}

	QString name = aName.toUpper();

	for (int i = 0; i < m_avScreenList.count(); ++i){
		if ( name != "APPVR" && (m_avScreenList.at(i) == name || name == "APPMEDIAPLAYER" || name == "APPMOBILETV")){
			QStringList avOff;
			avOff.append("COMMON");
			avOff.append("WARN");
			avOff.append("%AV OFF%");
			clearCommands(avOff);
			break;
		}
	}


	if ( (isFront && !m_front) || (!isFront && m_front) ){
		processOSD(0);	// need to check zero arg is right value
		return;
	}


	lprintf("checkScreen : %s", qPrintable(name));


	for (int i = 0; i < m_avScreenList.count(); ++i){
            if ( m_avScreenList.at(i) == name){

                lprintf("checkScreen m_avScreenList.at(%d):%s",i,qPrintable(m_avScreenList.at(i)));

                if (m_dataStack.count() > 0) {
                    OSDData * data = m_dataStack.last();

                    //lprintf("clearCommands findClear: %s, %s", qPrintable(data->getCommand().at(0)), qPrintable(data->getCommand().at(1)));

                    if (data != NULL && name == "APPVR" && data->getCommand().at(0) == "DMB" && data->getDataType() == OSDData::Stay) {
                        clearAV_exceptDMB();
                    }
                    else {
                        clearAV();
                    }
                }
                else {
                    clearAV();
                }

                break;
            }
	}

	for (int i = 0; i < m_infoScreenList.count(); ++i){
		if ( m_infoScreenList.at(i) == name){
			clearInfo();
			break;
		}
	}

	m_currentScreen = name;
	processOSD(0);
}

void OSDModel::clearAV(){
	lprintf("clear av");
	clear(m_avCommandList);
}

void OSDModel::clearAV_exceptDMB(){
        lprintf("clear av dmb");
        clear(m_avCommandClearList);
}

void OSDModel::clearBluetooth(){
	lprintf("clear bluetooth");
	clear(m_bluetoothCommandList);
}

void OSDModel::clearInfo(){
	lprintf("clear info");
	clear(m_infoCommandList);
}

void OSDModel::clear(const QStringList & list){
	for (int i = 0; i < list.count(); ++i){
		QStringList commands;
		commands.append(list.at(i));
		clearCommands(commands);
	}
}

void OSDModel::osdTimeout(){
	OSDData * data = qobject_cast<OSDData *>(sender());
	int prevCount = m_dataStack.count();
	m_dataStack.removeOne(data);
	delete data;
	processOSD(prevCount);
}

void OSDModel::activateOSD(){
	m_activate = true;
}
