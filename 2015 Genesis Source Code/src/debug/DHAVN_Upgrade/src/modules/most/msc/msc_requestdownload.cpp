#include "msc_requestdownload.h"


MscRequestDownload::MscRequestDownload(QObject * obj, QDBusAbstractInterface * p, SWDLID::SWDLID swdlid)
: MscAbstract(obj, p, swdlid){
	connect(getProxy(), SIGNAL(RequestDownload(const QDBusVariant &)), SLOT(RequestDownload(const QDBusVariant &)));
}

MscRequestDownload::~MscRequestDownload() {}

void MscRequestDownload::start(){
	startRequestDownload();
}

void MscRequestDownload::startRequestDownload(){
	struct SRequestDownload sd;
	QDBusVariant variant;
	sd.status = STATUS_STARTRESULTACK;
	sd.swdlid = getSwdlId();
	sd.DeviceID = DEVICE_ID;
	sd.senderhandle = SENDER_HANDLE;

	// originally we use multiple modules, but we assume that exist only one modules in DHAVN
	sd.moduletype = MscAbstract::Modules()[0]->moduleType;
	sd.moduleindex = MscAbstract::Modules()[0]->moduleIndex;	
	sd.blocksize = 0x8000;	
	sd.swdlchannel = 0x02;
	variant.setVariant(QVariant::fromValue(sd));

	mscTrigger(SWDLID::RequestDownload);
	getProxy()->SetRequestDownload(variant);
}

void MscRequestDownload::RequestDownload(const QDBusVariant & variant){
	struct SRequestDownload sd;
	QVariant v = variant.variant();
	QDBusArgument argument;
	int result = -1;
	argument = v.value<QDBusArgument>();

	argument >> sd;

	if ( sd.status == STATUS_RESULTACK){
		ULOG->log("RequestDownload : moduleType : %d, moduleIndex : %d, moduleBlock : %d, swdlchannel : %d", sd.moduletype, sd.moduleindex, sd.blocksize, sd.swdlchannel);
		QList<ModuleVersion *> & modules = MscAbstract::Modules();

		// 2012.04.09 it doesn't need to compare because IBox doesn't return moduletype and moduleindex(see MSC), not yet modified
		for ( int i = 0; i < modules.count(); ++i){
			ULOG->log("Compare : (%d, %d), (%d, %d)", modules.at(i)->moduleType, sd.moduletype,
													modules.at(i)->moduleIndex, sd.moduleindex);
			if ( modules.at(i)->moduleType == sd.moduletype && modules.at(i)->moduleIndex == sd.moduleindex){
				ULOG->log("found module match");
				modules[i]->blocksize = sd.blocksize;
				modules[i]->swdlchannel = sd.swdlchannel;
				result = i;
				break;
			}
		}

		if ( result == -1){
			emit statusChanged(SWDLID::RequestDownload, SWDLID::Receive, SWDLID::StatusError, QString("cannot found match module"));
			return;
		}

		emit statusChanged(SWDLID::RequestDownload, SWDLID::Receive, SWDLID::StatusSuccess, QString::number(result));
		startNext();
	}
	else{
		emit statusChanged(SWDLID::RequestDownload, SWDLID::Receive, SWDLID::StatusError, QString("Not ResultAck"));
	}
}
