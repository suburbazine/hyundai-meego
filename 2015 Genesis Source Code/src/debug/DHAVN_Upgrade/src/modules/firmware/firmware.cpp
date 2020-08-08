#include "firmware.h"

#include "common.h"

typedef struct{
	const char * firmware_name;
	const char * launcher;
	Firmware::DependencyCode dependency;
	unsigned int weight;
} FirmwareAttribute;

static const FirmwareAttribute firmware_attributes[] = {
	{ "bt",				"bt_fw_updater",		Firmware::DEP_NONE, 140 },
	{ "ccp",			"control_fw_updater",	Firmware::DEP_MICOM, 275},
	{ "rrc",			"control_fw_updater",	Firmware::DEP_MICOM, 250},
	{ "hdradio",		"hdradio_fw_updater",	Firmware::DEP_MICOM, 150},
	{ "dab",			"dab_fw_updater"	,	Firmware::DEP_NONE, 90},
	{ "fpga",			"fpga_fw_updater"	,	Firmware::DEP_NONE, 80},
	{ "inic",			"inic_fw_updater"	,	Firmware::DEP_MOST_END, 390},
	{ "tdmb",			"tdmb_fw_updater"	,	Firmware::DEP_NONE, 90 },
	{ "micom",			"micom_fw_updater"	,	Firmware::DEP_MOST_END, 120},
	{ "gps",			"gps_fw_updater"	,	Firmware::DEP_NONE, 240},
	{ "xm",				"xm_fw_updater"		,	Firmware::DEP_NONE, 200},
	{ "bios",			"bios_fw_updater"	,	Firmware::DEP_ALL_FINISHED, 60},
	{ "front_lcd", 		"lcd_fw_updater"	,	Firmware::DEP_MICOM, 130},
	{ "rear_right_lcd",	"lcd_fw_updater" 	,	Firmware::DEP_MICOM, 130},
	{ "rear_left_lcd",	"lcd_fw_updater"	,	Firmware::DEP_MICOM, 130},
        { "ssd",	        "ssd_fw_updater"	        ,	Firmware::DEP_SSD, 60}, // SANGWOO_TEMP
	{ "gnss",			"gnss_fw_updater"	,	Firmware::DEP_NONE, 240},
	{ NULL,		NULL,	Firmware::DEP_NONE, 0}
};

QString Firmware::GetLauncherByName(const QString & name){
	int index = 0;
	while(1){
		if ( firmware_attributes[index].firmware_name == NULL)
			break;
		else if ( firmware_attributes[index].firmware_name == name.toLower())
			return QString(firmware_attributes[index].launcher);

		++index;
	}
	return QString();
}

Firmware::DependencyCode Firmware::GetDependencyByName(const QString & name){
	int index = 0;
	while(1){
		if ( firmware_attributes[index].firmware_name == NULL)
			break;
		else if ( firmware_attributes[index].firmware_name == name.toLower())
			return firmware_attributes[index].dependency;

		++index;
	}
	return Firmware::DEP_NONE;
}

unsigned int Firmware::GetWeightByName(const QString & name){
	int index = 0;
	while(1){
		if ( firmware_attributes[index].firmware_name == NULL)
			break;
		else if ( firmware_attributes[index].firmware_name == name.toLower())
			return firmware_attributes[index].weight;
		
		++index;
	}
	return 0;
}

Firmware::Firmware(QObject * obj)
: QObject(obj){
	m_isValid = true;
	m_versionSeparator = QChar('.');
	m_base = 16;
}

void Firmware::setFileVersion(const QString & ver){
	bool ok;
	m_fileVersion.clear();

	QStringList list = ver.split(m_versionSeparator);

	for (int i = 0; i < list.count(); ++i){
		unsigned char verToken = (unsigned char) list.at(i).toInt(&ok, m_base);

		if ( !ok ){
			m_fileVersion.clear();
			m_isValid = false;
			break;
		}

		m_fileVersion.append(verToken);
	}	
}

void Firmware::setFirmwareVersion(const QByteArray & ar){
	m_firmwareVersion.clear();
	m_firmwareVersion.append(ar);
}

int Firmware::compare(){
	if ( !valid())
		return 0;

	if(m_fileVersion.count() > m_firmwareVersion.count())
		return 0;

	int fileVersion = 0;
	int firmwareVersion = 0;
	const int count = m_fileVersion.count();

	int index = 0;
	for (int i = count - 1; i >= 0; --i){
		unsigned int fileVer = (unsigned int)m_fileVersion.at(index);
		unsigned int firmwareVer = (unsigned int)m_firmwareVersion.at(index);
		fileVersion |= ( fileVer << (8 * i));
		firmwareVersion |= (firmwareVer << (8 * i));

		index++;
	}

	if ( fileVersion > firmwareVersion)
		return 1;
	else if ( fileVersion < firmwareVersion)
		return -1;

	return 0;
}
