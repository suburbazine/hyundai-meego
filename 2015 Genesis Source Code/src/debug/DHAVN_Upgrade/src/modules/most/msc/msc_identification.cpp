#include "msc_identification.h"

MscIdentification::MscIdentification(QObject * obj, QDBusAbstractInterface * p, SWDLID::SWDLID swdlid)
: MscAbstract(obj, p, swdlid){
	connect(getProxy(), SIGNAL(ReadHWIdentifier(uint, const QString &)), SLOT(ReadHWIdentifier(uint, const QString &)));
	connect(getProxy(), SIGNAL(ReadDevName(uint, const QString &)), SLOT(ReadDevName(uint, const QString &)));
	connect(getProxy(), SIGNAL(ReadHWVersion(uint, const QString &)), SLOT(ReadHWVersion(uint, const QString &)));
	connect(getProxy(), SIGNAL(ReadNumberofModule(uint, const QByteArray &)), SLOT(ReadNumberofModule(uint, const QByteArray &)));
	connect(getProxy(), SIGNAL(ReadModuleVersion(const uint, const QDBusVariant &)), SLOT(ReadModuleVersion(const uint, const QDBusVariant &)));

}

MscIdentification::~MscIdentification(){ }

void MscIdentification::start(){
	if ( getSwdlId() == SWDLID::IBOX)
		readHWIdentifier();
	else
		readHWVersion();
}

void MscIdentification::readHWIdentifier(){
	mscTrigger(SWDLID::IdentificationHWIdentifier);	
	getProxy()->SetReadHWIdentifier(getSwdlId(), MOSTDef::ID_STATE_GET, ""); 
}

void MscIdentification::readHWVersion(){
	mscTrigger(SWDLID::IdentificationHWVersion);
	getProxy()->SetReadHWVersion(getSwdlId(), "");
}

void MscIdentification::readDevName(){
	mscTrigger(SWDLID::IdentificationDevName);
	getProxy()->SetReadDevName(getSwdlId(), "");
}

void MscIdentification::readNumberOfModule(){
	mscTrigger(SWDLID::IdentificationNumberOfModule);
	getProxy()->SetReadNumberofModule(getSwdlId(), QByteArray());
}

void MscIdentification::readModuleVersion(){
	struct SModuleVersionListInfo versionListInfo;
	struct SModuleVerisonList versionList;
	QDBusVariant dvariant;
	QVariant	variant;
	QDBusArgument dargument;

	versionListInfo.infosize = 0;
	versionListInfo.moduleversionlistinfo.clear();
	dargument << versionListInfo;
	variant.setValue(dargument);
	dvariant.setVariant(variant);

	mscTrigger(SWDLID::IdentificationReadModuleVersion);
	getProxy()->SetReadModuleVersion(getSwdlId(), dvariant);
}

void MscIdentification::ReadHWIdentifier(uint, const QString & str){
	QString d = ":identifier(" + str + ")";
	emit statusChanged(SWDLID::IdentificationHWIdentifier, 
							SWDLID::Receive,
							SWDLID::StatusSuccess,
							d);
	readDevName();
}

void MscIdentification::ReadDevName(uint id, const QString & str){
	Q_UNUSED(id);
	QString d = ":devname(" + str + ")";
	emit statusChanged(SWDLID::IdentificationDevName,
						SWDLID::Receive,
						SWDLID::StatusSuccess, d);
	readHWVersion();	
}

void MscIdentification::ReadHWVersion(uint id, const QString  & str){
	Q_UNUSED(id);

	QString d = ":hwversion(" + str + ")";
	emit statusChanged(SWDLID::IdentificationHWVersion,
						SWDLID::Receive,
						SWDLID::StatusSuccess, d);
	readNumberOfModule();
}

void MscIdentification::ReadNumberofModule(uint, const QByteArray & modulevalue){
	uint code_count = 0;
	uint data_count = 0;
	uint boot_count = 0;

	QString d;
	if ( modulevalue.count() == 3){
		code_count = (uint)modulevalue.at(0);
		data_count = (uint)modulevalue.at(1);
		boot_count = (uint)modulevalue.at(2);
	}
	else{
		d = QString("module count invalid : ") + QString::number(modulevalue.count());
		emit statusChanged(SWDLID::IdentificationNumberOfModule, SWDLID::Receive, SWDLID::StatusError, d);
		return;
	}

	d = ":codemodule(" 
			+ QString::number(code_count) 
			+ "):datamodule(" + QString::number(data_count) 
			+ "):bootmodule(" + QString::number(boot_count) 
			+ ")";
	emit statusChanged(SWDLID::IdentificationNumberOfModule, SWDLID::Receive,
			SWDLID::StatusSuccess, d);
	readModuleVersion();
}

void MscIdentification::ReadModuleVersion(uint, const QDBusVariant & variant){
	struct SModuleVersionListInfo versionListInfo;
	int moduleIndex[3] = {0};
	QVariant v = variant.variant();
	QDBusArgument argument;
	argument = v.value<QDBusArgument>();

	argument >> versionListInfo;

	QString d = QString(":modulecount(")
				+ QString::number(versionListInfo.infosize) 
				+ ")";


	for (int i = 0; i < (int)versionListInfo.infosize; ++i){
		ULOG->log("moduletype : %s, moduleversion : %s", qPrintable(versionListInfo.moduleversionlistinfo.at(i).moduletype),
						qPrintable(versionListInfo.moduleversionlistinfo.at(i).moduleversion));
		ModuleVersion * mv = new ModuleVersion;
		mv->moduleType = versionListInfo.moduleversionlistinfo.at(i).moduletype;
		if ( mv->moduleType < 1 || mv->moduleType > 3){
			ULOG_EXCEPTION; ULOG->log("module type error");
			delete mv;
			return;
		}

		moduleIndex[mv->moduleType - 1] += 1;
		mv->moduleVersion = versionListInfo.moduleversionlistinfo.at(i).moduleversion;
		mv->moduleIndex = moduleIndex[mv->moduleType - 1];
		mv->blocksize = 0;
		mv->swdlchannel = 0;
		MscAbstract::AddModule(mv);

		d += ":moduletype(" 
				+ QString::number(versionListInfo.moduleversionlistinfo.at(i).moduletype) 
				+ "):modulversion(" 
				+ versionListInfo.moduleversionlistinfo.at(i).moduleversion
				+ ")";
	}

	if ( versionListInfo.infosize == 0){
		emit statusChanged(SWDLID::IdentificationReadModuleVersion, SWDLID::Receive, SWDLID::StatusError, ":reason(module_count_zero)");
		return;
	}

	emit statusChanged(SWDLID::IdentificationReadModuleVersion, SWDLID::Receive,
			SWDLID::StatusSuccess, d);

	disconnect(getProxy(), SIGNAL(ReadHWIdentifier(uint, const QString &)), this, 0);
	startNext();
}

