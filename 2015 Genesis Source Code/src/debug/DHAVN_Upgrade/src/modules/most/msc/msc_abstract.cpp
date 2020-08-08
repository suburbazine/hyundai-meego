#include "msc_abstract.h"

QList<ModuleVersion *> MscAbstract::s_modules;

MscAbstract::MscAbstract(QObject * obj, QDBusAbstractInterface * p, SWDLID::SWDLID swdlid)
: QObject(obj){
	m_swdlid = swdlid;
	m_next = NULL;
	m_proxy = qobject_cast<local::CSWDLAdaptor *>(p);
	m_timer = new QTimer(this);
	m_timer->setSingleShot(true);
	connect(m_timer, SIGNAL(timeout()), SIGNAL(timeout()));
}

MscAbstract::~MscAbstract(){ 
	MscAbstract::DeleteAllModules();
}

void MscAbstract::setNextStep(MscAbstract * next){
	m_next = next;
}

local::CSWDLAdaptor * MscAbstract::getProxy(){
	return m_proxy;
}

SWDLID::SWDLID MscAbstract::getSwdlId(){
	return m_swdlid;
}

void MscAbstract::startNext(){
	if ( m_next )
		m_next->start();
}

void MscAbstract::stopTimer(){
	if ( m_timer->isActive())
		m_timer->stop();
}

void MscAbstract::mscTrigger(SWDLID::SEQUENCE sequence){
        //m_timer->start(3000);
    m_timer->start(10000);  
	switch(sequence){
	case SWDLID::IdentificationHWIdentifier:
		connect(getProxy(), SIGNAL(ReadHWIdentifier(uint, const QString &)), m_timer, SLOT(stop()));
		break;

	case SWDLID::IdentificationDevName:
		connect(getProxy(), SIGNAL(ReadDevName(uint, const QString &)), m_timer, SLOT(stop()));
		break;

	case SWDLID::IdentificationHWVersion:
		connect(getProxy(), SIGNAL(ReadHWVersion(uint, const QString &)), m_timer, SLOT(stop()));
		break;

	case SWDLID::IdentificationNumberOfModule:
		connect(getProxy(), SIGNAL(ReadNumberofModule(uint, const QByteArray &)), m_timer, SLOT(stop()));
		break;

	case SWDLID::IdentificationReadModuleVersion:
		connect(getProxy(), SIGNAL(ReadModuleVersion(const uint, const QDBusVariant &)), m_timer, SLOT(stop()));
		break;

	case SWDLID::RequestDownload:
		connect(getProxy(), SIGNAL(RequestDownload(const QDBusVariant &)), m_timer, SLOT(stop()));
		break;

	case SWDLID::PrepareFileTransfer:
		connect(getProxy(), SIGNAL(PrepareFileTransfer(const QDBusVariant &)), m_timer, SLOT(stop()));
		break;

	case SWDLID::RequestTransferExit:
		connect(getProxy(), SIGNAL(RequestTransferExit(const QDBusVariant &)), m_timer, SLOT(stop()));
		break;

	case SWDLID::CheckActivateModule:
		connect(getProxy(), SIGNAL(CheckActivateModule(const QDBusVariant &)), m_timer, SLOT(stop()));
		break;

	case SWDLID::ReprogrammingReprogramingMode:
		connect(getProxy(), SIGNAL(ReprogrammingMode(const QDBusVariant &)), m_timer, SLOT(stop()));
		break;

	case SWDLID::ReprogrammingNormalOperationMode:
		connect(getProxy(), SIGNAL(ReprogrammingMode(const QDBusVariant &)), m_timer, SLOT(stop()));
		break;

	default: break;
	}

	emit statusChanged(sequence, SWDLID::Send);
}
