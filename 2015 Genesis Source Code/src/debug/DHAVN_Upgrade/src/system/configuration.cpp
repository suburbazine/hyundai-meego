#include "configuration.h"

#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QCryptographicHash>

#include "common.h"
#include "updateelement.h"
#include "checkessentials.h"

#define HASH_SIZE	20
static const unsigned char p1[HASH_SIZE] = {0x41, 0x3C, 0x23, 0x53, 0xF7, 0xB5, 0x75, 0x1C, 0xAA, 0xE4, 0xBE, 0xFD, 0xEE, 0x0C, 0xED, 0xE7, 0x72, 0x55, 0x7E, 0xB7};
static const unsigned char p2[HASH_SIZE] = {0xB0, 0x24, 0x61, 0x0A, 0xCC, 0x90, 0x14, 0xA9, 0x3F, 0x1F, 0x70, 0x6D, 0x44, 0x41, 0x6D, 0xA3, 0x27, 0x33, 0xCE, 0x57};

Configuration::Configuration(QObject * obj)
: QObject(obj){
	m_compareMode = UpperVersion;
	m_user = UserMode;
}

bool Configuration::checkUser(){
	if ( m_user == DeveloperMode  || m_user == XMode){
		QCryptographicHash hash(QCryptographicHash::Sha1);
		hash.addData(qPrintable(m_password), m_password.length());
		QByteArray result = hash.result();

		const unsigned char * h;
		if ( m_user == DeveloperMode)
			h = p1;
		else
			h = p2;
			
		for (int i = 0; i < HASH_SIZE; ++i)
			if ( h[i] != (unsigned char) result.at(i))
				return false;
	}

	return true;
}

bool Configuration::checkEssentialKey(const QString & mountPoint){
	if ( m_user == UserMode){
		CheckEssentials essentials(mountPoint, m_essentialKey);
		if ( !essentials.isEssentialValidate())
			return false;
	}

	return true;
}

bool Configuration::checkUpdateKey(const QString & imageSizeString){
	char sha1Buf[HASH_SIZE * 2 + 1] = {0};
	int index = 0;
	
	QString key = imageSizeString + m_essentialKey + m_buildDate;
	QCryptographicHash hash(QCryptographicHash::Sha1);
	hash.addData(qPrintable(key), key.length());
	QByteArray result = hash.result();

	for (int i = 0; i < result.count(); ++i){
		snprintf(sha1Buf + index, 3, "%02x", (unsigned char) result.at(i));
		index += 2;
	}

	ULOG->log("sha1Buf : %s", sha1Buf);

	if ( QString(sha1Buf).toLower() == m_mainKey.toLower())
		return true;

	return false;	
}

void Configuration::setCompareMode(const QString & str){
	// If not specifying mode, default is NoVersion and it has no limit, it will be changed in the future
	ULOG->log("COMPARE MODE(%s)", qPrintable(str));
	if ( str == "same")
		m_compareMode = SameVersion;
	else if ( str == "upper")
		m_compareMode = UpperVersion;
	else if ( str == "lower")
		m_compareMode = LowerVersion;
	else if ( str == "diff")
		m_compareMode = DiffVersion;
	else
		m_compareMode = NoVersion;
}

void Configuration::setUpdateMode(const QString & str){
	if ( str == "user")
		m_user = UserMode;
	else if ( str == "developer")
		m_user = DeveloperMode;
	else if ( str == "xmode")
		m_user = XMode;
	else
		m_user = UserMode;
}

bool Configuration::validateConfiguration(){
	if ( m_user == UserMode ){
		if ( m_essentialKey.isNull() || m_essentialKey.isEmpty())
			return false;
	}
	else if ( m_user == DeveloperMode){
		if ( m_password.isNull() || m_password.isEmpty())
			return false;
	}

	return true;
}

bool Configuration::readConfiguration(const QString & path){
	QFile f (path + CONFIGURATION_PATH);

	if ( !f.open(QIODevice::ReadOnly)){
		ULOG_EXCEPTION;
		return false;
	}
		
	QDomDocument doc("dhavn");

	if ( !doc.setContent(&f)){
		f.close();
		ULOG_EXCEPTION;
		return false;
	}

	f.close();

	QDomElement docElement = doc.documentElement();	

	if ( docElement.tagName().toLower() == "dhavn"){
		QDomElement element = docElement.firstChild().toElement();
		while ( ! element.isNull()){
			QDomText value = element.firstChild().toText();
			QString eleName = element.tagName().toLower();
			QString valueData = value.data().trimmed();

			if ( !value.isNull()){
				if ( eleName == "version"){
					ULOG->log("UPDATE VERSION : %s", qPrintable(valueData));
					m_requireVersion = element.attribute("require").trimmed();
					if ( !m_requireVersion.isEmpty())
						ULOG->log("Require Version : %s", qPrintable(m_requireVersion));

					m_updateVersion = valueData;
					setCompareMode(element.attribute("compare").toLower());
				}
				else if ( eleName == "upgrade-mode"){
					setUpdateMode(valueData);
				}
				else if ( eleName == "password"){
					m_password = valueData;
				}
				else if ( eleName == "firmware"){
					QString ver = element.attribute("version").trimmed();
					QString path = element.attribute("src").trimmed();
                                        QString lname = element.attribute("lcdtype").trimmed();

					if ( !path.isEmpty()){
						m_targetList.append(
								UpdateElement::CreateFirmwareElement( 
                                                                valueData, ver, path, element.attribute("variant").trimmed(),lname));
					}
				}
				else if ( eleName == "build-date"){
					m_buildDate = valueData;
				}
				else if ( eleName == "essentials"){
					m_essentialKey = valueData;
				}
				else if ( eleName == "upgrade-key"){
					m_mainKey = valueData;
				}
			}
			else{	// no value
				if ( eleName == "ibox"){
                                    ULOG->log(" IBOX VARIANT : %s", qPrintable(element.attribute("variant").trimmed()));
                                    ULOG->log(" IBOX VARIANT g_variantCode : %d", g_variantCode);
                                    ULOG->log(" IBOX VARIANT name : %s", qPrintable(g_variant_str[g_variantCode]));
                                    if(element.attribute("variant").trimmed() == g_variant_str[g_variantCode]){
                                        ULOG->log("IBOX VARIANT MATCHING");
                                        UpdateElement * ue = UpdateElement::CreateIBoxElement(
                                                                element.attribute("version").trimmed(),
                                                                element.attribute("date").trimmed());

                                        m_targetList.append(ue);
                                    }else if(element.attribute("variant").trimmed() == NULL){
                                         ULOG->log("IBOX VARIANT DEFAULT");
                                        UpdateElement * ue = UpdateElement::CreateIBoxElement(
                                                                element.attribute("version").trimmed(),
                                                                element.attribute("date").trimmed());

                                        m_targetList.append(ue);
                                    }else{
                                        ULOG->log("IBOX VARIANT NOT MATCH");
                                    }
				}
				else if ( eleName == "amp"){
					UpdateElement * ue = UpdateElement::CreateAMPElement(
								element.attribute("version").trimmed());
					m_targetList.append(ue);
				}
			}
			element = element.nextSibling().toElement();
		}
	}
	else{
		ULOG_EXCEPTION;
		return false;
	}

	return true;
}
