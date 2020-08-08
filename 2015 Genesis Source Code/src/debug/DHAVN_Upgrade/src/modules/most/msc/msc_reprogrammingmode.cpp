#include "msc_reprogrammingmode.h"

MscReprogrammingMode::MscReprogrammingMode(QObject * obj, QDBusAbstractInterface * p, SWDLID::SWDLID swdlid)
: MscAbstract(obj, p, swdlid){
	m_tryCount = 0;
        m_bootloaderTimer = NULL;

	connect(getProxy(), SIGNAL(ReprogrammingMode(const QDBusVariant &)), SLOT(ReprogrammingMode(const QDBusVariant &)));
}

MscReprogrammingMode::~MscReprogrammingMode(){ }

void MscReprogrammingMode::start(){
	startReprogramming();
}

void MscReprogrammingMode::startReprogramming(){
	QDBusVariant variant;

	struct SReporgrammingMode smode;
	smode.status = STATUS_STARTRESULTACK;
	smode.swdlid = getSwdlId();
	smode.DeviceID = DEVICE_ID;
	smode.senderhandle = SENDER_HANDLE;
	smode.SWDLmode = REPROGRAMMING_REPROGRAMMING_MODE;	
	variant.setVariant(QVariant::fromValue(smode));

	mscTrigger(SWDLID::ReprogrammingReprogramingMode);
	getProxy()->SetReprogrammingMode(variant);
}

void MscReprogrammingMode::waitEnterBootloader(){
	m_bootloaderTimer = new QTimer(this);
	m_bootloaderTimer->setSingleShot(true);
	connect(m_bootloaderTimer, SIGNAL(timeout()), SLOT(notReceiveAck()));
	connect(getProxy(), SIGNAL(ReadHWIdentifier(uint, const QString &)), SLOT(ReadHWIdentifier(uint, const QString &)));

	getProxy()->SetReadHWIdentifier(getSwdlId(), MOSTDef::ID_STATE_GET, "");
	m_bootloaderTimer->start(1000);
}

void MscReprogrammingMode::notReceiveAck(){
	++m_tryCount;
	ULOG->log("AMP Not Receive Ack : %d", m_tryCount);

	if ( m_tryCount >= 10)	
		emit timeout();
	else
		getProxy()->SetReadHWIdentifier(getSwdlId(), MOSTDef::ID_STATE_GET, "");
}

void MscReprogrammingMode::ReadHWIdentifier(uint, const QString & str){
	if ( m_bootloaderTimer->isActive()) m_bootloaderTimer->stop();

	startNext();
}

void MscReprogrammingMode::ReprogrammingMode(const QDBusVariant & variant){
	struct SReporgrammingMode smode;
	QVariant v = variant.variant();
	QDBusArgument arg;
	arg = v.value<QDBusArgument>();
	arg >> smode;

	if ( smode.SWDLmode == REPROGRAMMING_REPROGRAMMING_MODE){
		stopTimer();

		if ( smode.status == STATUS_RESULTACK){
			emit statusChanged(SWDLID::ReprogrammingReprogramingMode, SWDLID::Receive, SWDLID::StatusSuccess, MscAbstract::IntegerToAck(smode.status));
			if ( getSwdlId() == SWDLID::IBOX)
				startNext();
			else
				QTimer::singleShot(2500, this, SLOT(waitEnterBootloader()));
		}
		else if ( smode.status == STATUS_ERROR){
			emit statusChanged(SWDLID::ReprogrammingReprogramingMode, SWDLID::Receive, SWDLID::StatusError, MscAbstract::IntegerToAck(smode.status));
		}
	}
}
