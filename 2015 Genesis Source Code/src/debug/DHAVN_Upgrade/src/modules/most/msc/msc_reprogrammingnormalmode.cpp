#include "msc_reprogrammingnormalmode.h"

MscReprogrammingNormalMode::MscReprogrammingNormalMode(QObject * obj, QDBusAbstractInterface * p, SWDLID::SWDLID swdlid)
: MscAbstract(obj, p, swdlid){

}

MscReprogrammingNormalMode::~MscReprogrammingNormalMode(){}

void MscReprogrammingNormalMode::start(){
	connect(getProxy(), SIGNAL(ReprogrammingMode(const QDBusVariant &)), SLOT(ReprogrammingMode(const QDBusVariant &)));

	startReprogrammingNormal();
}

void MscReprogrammingNormalMode::startReprogrammingNormal(){
	QDBusVariant variant;

	struct SReporgrammingMode smode;
	smode.status = STATUS_STARTRESULTACK;
	smode.swdlid = getSwdlId();
	smode.DeviceID = DEVICE_ID;
	smode.senderhandle = SENDER_HANDLE;
	smode.SWDLmode = REPROGRAMMING_NORMAL_OPERATION_MODE;
	variant.setVariant(QVariant::fromValue(smode));

	mscTrigger(SWDLID::ReprogrammingNormalOperationMode);
	getProxy()->SetReprogrammingMode(variant);
}

void MscReprogrammingNormalMode::ReprogrammingMode(const QDBusVariant & variant){
	struct SReporgrammingMode smode;
	QVariant v = variant.variant();
	QDBusArgument arg;
	arg = v.value<QDBusArgument>();
	arg >> smode;

	if ( smode.SWDLmode == REPROGRAMMING_NORMAL_OPERATION_MODE){
		stopTimer();

		if ( smode.status == STATUS_RESULTACK){
			emit statusChanged(SWDLID::ReprogrammingNormalOperationMode, SWDLID::Receive, SWDLID::StatusSuccess, MscAbstract::IntegerToAck(smode.status));
			startNext();		
		}
		else if ( smode.status == STATUS_ERROR){
			emit statusChanged(SWDLID::ReprogrammingNormalOperationMode, SWDLID::Receive, SWDLID::StatusError, MscAbstract::IntegerToAck(smode.status));
		}
	}
}
