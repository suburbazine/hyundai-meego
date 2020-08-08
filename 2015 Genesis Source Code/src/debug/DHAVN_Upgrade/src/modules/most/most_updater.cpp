#include "most_updater.h"
#include "updateenvironment.h"

#include "common.h"
#include "meegoeuid.h"

#include "msc/msc_identification.h"
#include "msc/msc_reprogrammingmode.h"
#include "msc/msc_requestdownload.h"
#include "msc/msc_filetransfer.h"
#include "msc/msc_reprogrammingnormalmode.h"
#include "msc/msc_finish.h"

MOSTUpdater::MOSTUpdater(QObject * obj)
: AbstractUpdater(AbstractUpdater::MOSTLauncher, obj){
	MeegoEUID euid;
	m_iboxUpdate = NULL;
	m_ampUpdate = NULL;
	m_currentSteps = NULL;
	m_currentUpdate = SWDLID::IBOX;

	m_swdlInterface = new local::CSWDLAdaptor("com.dhavn.mostmanager.qtdbus", "/swdl", QDBusConnection::sessionBus());
}

MOSTUpdater::~MOSTUpdater() {}

void MOSTUpdater::setUpdateList(const QList<UpdateElement *> & list){
	for (int i = 0; i < list.count(); ++i){
		if ( list.at(i)->type() == UpdateElement::et_IBOX)
			m_iboxUpdate = list[i];
		else if ( list.at(i)->type() == UpdateElement::et_AMP)
			m_ampUpdate = list[i];
	}
}

void MOSTUpdater::start(){
	if ( m_iboxUpdate == NULL && m_ampUpdate == NULL){
		ULOG->log("MOSTUpdate list zero");
		emit finished(0);
		return;
	}

	if ( m_iboxUpdate )
		startIBoxUpdate();
	else if ( m_ampUpdate )
		startAmplifierUpdate();
}

void MOSTUpdater::startIBoxUpdate(){
	ULOG->log("Start IBOX Update");
	m_currentSteps = createSteps(SWDLID::IBOX);
	
	for (int i = 0; i < NumberOfSteps; ++i){
		connect(m_currentSteps[i], SIGNAL(statusChanged(int,int,int,const QString &)), SLOT(statusChanged(int,int,int,const QString &)));
		connect(m_currentSteps[i], SIGNAL(timeout()), SLOT(cannotReceiveResponse()));
	}
	m_currentSteps[0]->start();
}

void MOSTUpdater::statusChanged(int step, int direction, int status, const QString & msg){
	ULOG->log("Step : %s, Direction : %d, Status : %d, Msg : %s", qPrintable(sequenceToString(step)), direction, status, qPrintable(msg));
	if ( status == SWDLID::StatusError ){
		if ( m_currentUpdate == SWDLID::IBOX){
			m_iboxUpdate->setStatus(UpdateElement::es_FAIL);
			m_iboxUpdate->setDescription(msg);
		}
		else{
			m_ampUpdate->setStatus(UpdateElement::es_FAIL);
			m_ampUpdate->setDescription(msg);
		}


		finishCurrentUpdate(true, msg);
	}
	else{
		if ( direction == SWDLID::Receive){
			int progress = sequenceToPercentage(step);
			if ( progress != -1){
                            UpdateElement * m_CurrentUpdate;

                            if ( m_currentUpdate == SWDLID::IBOX){
                                m_CurrentUpdate = m_iboxUpdate;
                            }
                            else{
                                m_CurrentUpdate = m_ampUpdate;
                            }
                                //emit updateProgressChanged(getCurrentUpdateName(), sequenceToString(step), progress, 100);
                           
                            emit updateProgressChanged(getCurrentUpdateName(), sequenceToString(step), progress, m_CurrentUpdate->weight());
			}
		}	
	}
}

void MOSTUpdater::cannotReceiveResponse(){
	ULOG->log("Cannot Receive Response");
	finishCurrentUpdate(true, "Timeout receiving response");
}

void MOSTUpdater::startAmplifierUpdate(){
	m_currentUpdate = SWDLID::AMP;

	if ( m_currentSteps != NULL)
		for (int i = 0; i < NumberOfSteps; ++i)
			delete m_currentSteps[i];

	m_currentSteps = createSteps(SWDLID::AMP);

	for (int i = 0; i < NumberOfSteps; ++i){
		connect(m_currentSteps[i], SIGNAL(statusChanged(int,int,int,const QString &)), SLOT(statusChanged(int,int,int,const QString &)));
		connect(m_currentSteps[i], SIGNAL(timeout()), SLOT(cannotReceiveResponse()));
	}
	m_currentSteps[0]->start();
}

void MOSTUpdater::stop(){

}

bool MOSTUpdater::finish(){
	removeDir("/common_data/upgrade/IBOX");
	removeDir("/common_data/upgrade/AMP");
	return true;	
}

void MOSTUpdater::finishCurrentUpdate(bool isError, const QString & msg){
       UpdateElement * m_CurrentUpdate;

       if ( m_currentUpdate == SWDLID::IBOX){
          m_CurrentUpdate = m_iboxUpdate;
        }
       else{
          m_CurrentUpdate = m_ampUpdate;
        }

	if ( isError ){
                //emit updateProgressChanged(getCurrentUpdateName(), msg, 100, 100);
            emit updateProgressChanged(getCurrentUpdateName(), msg, 100, m_CurrentUpdate->weight());
	}
	else{
                //emit updateProgressChanged(getCurrentUpdateName(), "Update Finished", 100, 100);
                emit updateProgressChanged(getCurrentUpdateName(), "Update Finished", 100, m_CurrentUpdate->weight());
	}

	if ( m_currentUpdate == SWDLID::IBOX){
		m_iboxUpdate->setStatus((isError?UpdateElement::es_FAIL:UpdateElement::es_DONE));
		m_iboxUpdate->setDescription(msg);
		if ( m_ampUpdate ){
			startAmplifierUpdate();
		}
		else{
			if ( isError )
				emit finished(1);
			else
				emit finished(0);
		}
	}
	else{
		m_ampUpdate->setStatus((isError?UpdateElement::es_FAIL:UpdateElement::es_DONE));
		m_ampUpdate->setDescription(msg);
		if ( isError )
			emit finished(1);
		else
			emit finished(0);
	}
}

void MOSTUpdater::blockTransfered(quint64 current, quint64 all){
	// AMP : 9 ~ 99, IBOX : 9 ~ 49
	int transferLimit = (m_currentUpdate == SWDLID::IBOX ? 40 : 90);
	double progress = (double) current / (double)all * 100.0;
	progress = progress * (double)transferLimit / 100.0;

	int progressInt = (int)progress;
	progressInt += 9;

	ULOG->log("MOST Block Transfered : %d", progressInt);

        UpdateElement * m_CurrentUpdate;

        if ( m_currentUpdate == SWDLID::IBOX){
            m_CurrentUpdate = m_iboxUpdate;
        }
        else{
            m_CurrentUpdate = m_ampUpdate;
        }
        
        //emit updateProgressChanged(getCurrentUpdateName(), "Transfering file block", progressInt, 100);
        emit updateProgressChanged(getCurrentUpdateName(), "Transfering file block", progressInt, m_CurrentUpdate->weight());
}

MscAbstract ** MOSTUpdater::createSteps(SWDLID::SWDLID swdlid){
	MscAbstract ** mscAbstract = new MscAbstract*[NumberOfSteps];

	mscAbstract[0] = new MscIdentification(this, m_swdlInterface, swdlid);
	mscAbstract[1] = new MscReprogrammingMode(this, m_swdlInterface, swdlid);
	mscAbstract[2] = new MscRequestDownload(this, m_swdlInterface, swdlid);
	mscAbstract[3] = new MscFileTransfer(this, m_swdlInterface, swdlid);
	mscAbstract[4] = new MscReprogrammingNormalMode(this, m_swdlInterface, swdlid);
	mscAbstract[5] = new MscFinish(this, m_swdlInterface, swdlid);

	connect(mscAbstract[3], SIGNAL(transfered(quint64, quint64)), SLOT(blockTransfered(quint64, quint64)));
	connect(mscAbstract[5], SIGNAL(updateFinished()), SLOT(finishCurrentUpdate()));
	
	if ( swdlid == SWDLID::IBOX)
		connect(mscAbstract[5], SIGNAL(iBoxWaitCount(int)), SLOT(processIBoxWaitCount(int)));


	for (int i = 0; i < (NumberOfSteps - 1); ++i)
		mscAbstract[i]->setNextStep(mscAbstract[i + 1]);

	return mscAbstract;
}

void MOSTUpdater::processIBoxWaitCount(int count){
	ULOG->log("processIBoxWaitCount : %d", count);
        UpdateElement * m_CurrentUpdate;

        if ( m_currentUpdate == SWDLID::IBOX){
            m_CurrentUpdate = m_iboxUpdate;
        }
        else{
            m_CurrentUpdate = m_ampUpdate;
        }
        
        //emit updateProgressChanged(getCurrentUpdateName(), "Wait IBOX internal update", 49 + count, 100);
        emit updateProgressChanged(getCurrentUpdateName(), "Wait IBOX internal update", 49 + count, m_CurrentUpdate->weight());
}

QString MOSTUpdater::getCurrentUpdateName(){
	if ( m_currentUpdate == SWDLID::IBOX)
		return "IBOX";
	else
		return "AMP";
}

int MOSTUpdater::sequenceToPercentage(int seq){
	switch(seq){
	case SWDLID::Idle: 							return 0;
	case SWDLID::IdentificationHWIdentifier:	return 1;
	case SWDLID::IdentificationDevName:			return 2;
	case SWDLID::IdentificationHWVersion:		return 3;
	case SWDLID::IdentificationNumberOfModule:	return 4;
	case SWDLID::IdentificationReadModuleVersion: return 5;
	case SWDLID::ReprogrammingReprogramingMode:	return 6; 
	case SWDLID::RequestDownload:				return 7;
	case SWDLID::ReprogrammingNormalOperationMode:
	{
		if ( m_currentUpdate == SWDLID::IBOX) return 49;
		else return 99;
	}
	default: break;
	}
	return -1;
}

QString MOSTUpdater::sequenceToString(int seq){
	switch(seq){
	case SWDLID::Idle: 							return "Idle";
	case SWDLID::IdentificationHWIdentifier:	return "IdentificationHWIdentifier";
	case SWDLID::IdentificationDevName:			return "IdentificationDevName";
	case SWDLID::IdentificationHWVersion:		return "IdentificationHWVersion";
	case SWDLID::IdentificationNumberOfModule:	return "IdentificationNumberOfModule";
	case SWDLID::IdentificationReadModuleVersion: return "IdentificationReadModuleVersion";
	case SWDLID::ReprogrammingReprogramingMode:	return "ReprogrammingReprogramingMode";
	case SWDLID::ReprogrammingNormalOperationMode: return "ReprogrammingNormalOperationMode";
	case SWDLID::RequestDownload:				return "RequestDownload";
	case SWDLID::PrepareFileTransfer:			return "PrepareFileTransfer";
	case SWDLID::TransferData:					return "TransferData";
	case SWDLID::RequestTransferExit:			return "RequestTransferExit";
	case SWDLID::CheckActivateModule:			return "CheckActivateModule";
	default: break;
	}

	return "Unknown Sequence";
}
