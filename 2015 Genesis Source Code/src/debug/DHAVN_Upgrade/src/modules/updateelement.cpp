#include "updateelement.h"
#include "common.h"
#include "firmware.h"


UpdateElement::UpdateElement(ElementType aType){
	m_type = aType;
	m_status = UpdateElement::es_READY;

        m_rpmType = UpdateElement::rpm_UPDATE;
        m_weight = 0;
}

UpdateElement * UpdateElement::CreateFirmwareElement(const QString & aName, const QString & aVersion, const QString & aPath, const QString & aVariant, const QString & lname){
	UpdateElement * element = new UpdateElement(UpdateElement::et_FIRMWARE);

        ULOG->log("XML Firmware(%s) : ver(%s), src(%s), variant(%s),lname(%s)",
                                qPrintable(aName), qPrintable(aVersion), qPrintable(aPath), qPrintable(aVariant),qPrintable(lname));

        element->setLcdType(lname);
	element->setName(aName);
	element->setPath(aPath);
	element->setVersion(aVersion);
	element->setVariant(aVariant);
	
	element->setWeight(Firmware::GetWeightByName(aName));

	return element;
}

UpdateElement * UpdateElement::CreateIBoxElement(const QString & aVersion, const QString & aDate){
	UpdateElement * element = new UpdateElement(UpdateElement::et_IBOX);
	ULOG->log("XML IBOX : ver(%s), date(%s)", qPrintable(aVersion), qPrintable(aDate));
	element->setName("IBOX");
	element->setVersion(aVersion);
	element->setDate(aDate);
	element->setWeight(1200);	// 20 min

	return element;
}

UpdateElement * UpdateElement::CreateAMPElement(const QString & aVersion){
	UpdateElement * element = new UpdateElement(UpdateElement::et_AMP);
	ULOG->log("XML AMP : ver(%s)", qPrintable(aVersion));
	element->setName("AMP");
	element->setVersion(aVersion);
	element->setWeight(360);	// 6 min

	return element;
}

UpdateElement * UpdateElement::CreateRPMElement(const QString & aName, const QString & version, RPMType type){
	UpdateElement * element = new UpdateElement(UpdateElement::et_RPM);
	ULOG->log("Zypper Package(%s) : ver(%s)", qPrintable(aName), qPrintable(version));
	element->setName(aName);
	element->setVersion(version);
	element->setRPMType(type);
	element->setWeight(10);		// 10 sec
	return element;
}

UpdateElement * UpdateElement::CreatePartitionElement(const QString & name, const QString & prevVersion, const QString & version, const QString & fileSource, unsigned int weight){
	UpdateElement * element = new UpdateElement(UpdateElement::et_PARTITION);
	element->setName(name);
	element->setPrevVersion(prevVersion);
	element->setVersion(version);
	element->setPath(fileSource);
	element->setWeight(weight);

	return element;
}

bool UpdateElement::isValid() const {
	return true;
}
