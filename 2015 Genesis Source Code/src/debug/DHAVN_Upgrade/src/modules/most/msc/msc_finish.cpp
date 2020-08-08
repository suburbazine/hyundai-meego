#include "msc_finish.h"

MscFinish::MscFinish(QObject * obj, QDBusAbstractInterface * p, SWDLID::SWDLID swdlid)
: MscAbstract(obj, p, swdlid){
	m_waitCount = 50;
        m_waitTimer = NULL;
}

MscFinish::~MscFinish(){}

void MscFinish::start(){
	if ( getSwdlId() == SWDLID::IBOX){
		ULOG->log("Start IBox Finish");
		connect(getProxy(), SIGNAL(UpdateProgressingInfoStatus(uint,uint)), SLOT(UpdateProcessing(uint, uint)));

		m_waitTimer = new QTimer(this);
		m_waitTimer->setInterval(24000);
		connect(m_waitTimer, SIGNAL(timeout()), SLOT(checkFinishIBoxUpdate()));
		m_waitTimer->start();

		deleteIBoxFiles();
	}
	else{
		ULOG->log("Start AMP Finish");
		deleteAmplifierFiles();
		emit updateFinished();
	}	
}

void MscFinish::deleteIBoxFiles(){
	gf_remove_directory(IBOX_DIRECTORY);
}

void MscFinish::deleteAmplifierFiles(){
        gf_remove_directory(QString(AMP_DIRECTORY));
}

void MscFinish::checkFinishIBoxUpdate(){
	--m_waitCount;

	if ( m_waitCount <= 0){
		m_waitTimer->stop();
		emit updateFinished();
	}
	else{
		emit iBoxWaitCount(50 - m_waitCount);
	}
}

void MscFinish::UpdateProcessing(uint swdlid, uint status){
	ULOG->log("UpdateProcessing : swdlid : %u, status : %u", swdlid, status);
	if ( swdlid == SWDLID::IBOX && status == 0x02){
		if ( m_waitTimer->isActive()) m_waitTimer->stop();

		emit updateFinished();
	}
}
