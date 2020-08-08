#include "msc_filetransfer.h"
#include <QFileInfo>

MscFileTransfer::MscFileTransfer(QObject * obj, QDBusAbstractInterface * p, SWDLID::SWDLID swdlid)
: MscAbstract(obj, p, swdlid){
	m_files = NULL;
	retry_ibox = false;
	disconnect_count = 0;

	m_allBlockCount = m_currentBlockCount = 0;
        connect(getProxy(), SIGNAL(iBoxSWDLFBlockVisible(const bool)), SLOT(iBoxBlockVisibleSlot(const bool)));

	connect(getProxy(), SIGNAL(PrepareFileTransfer(const QDBusVariant &)), SLOT(PrepareFileTransfer(const QDBusVariant &)));
	connect(getProxy(), SIGNAL(RequestTransferExit(const QDBusVariant &)), SLOT(RequestTransferExit(const QDBusVariant &)));
	connect(getProxy(), SIGNAL(CheckActivateModule(const QDBusVariant &)), SLOT(CheckActivateModule(const QDBusVariant &)));

	connect(getProxy(), SIGNAL(TransferDataAMS(const QDBusVariant &)), SLOT(TransferData(const QDBusVariant &)));
        
        timer = new QTimer(this);
        timer->setSingleShot(true);
        if ( getSwdlId() == SWDLID::IBOX){

           // timer->setInterval(10000);
            connect(timer, SIGNAL(timeout()), SIGNAL(timeout()));
            connect(timer, SIGNAL(timeout()), SLOT(TimeoutRequestTransferExit()));
        }

}

MscFileTransfer::~MscFileTransfer() {}

void MscFileTransfer::start(){
	if ( getSwdlId() == SWDLID::IBOX)
		m_files = g_ibox_file_checksum;
	else
		m_files = g_amplifier_file_checksum;

	if ( m_files == NULL ){
		emit statusChanged(SWDLID::PrepareFileTransfer, SWDLID::Send, SWDLID::StatusError, QString("Cannot find files"));	
	}
	else{
		if ( getSwdlId() == SWDLID::IBOX)
			startPrepareFileTransferPassive();
		else
			startPrepareFileTransfer();
	}
}

MOSTFileCheckSum * MscFileTransfer::nextFile(){
	return m_files->next;
}
QDBusArgument argumentTD;
void MscFileTransfer::TransferData(const QDBusVariant & var){
	struct STransferDataCMSAMS sdca;

	QVariant v = var.variant();
	quint32 allBlockCount = m_files->fileSize / MscAbstract::Modules()[0]->blocksize;
	if ( m_files->fileSize % MscAbstract::Modules()[0]->blocksize)
		allBlockCount++;

        //QDBusArgument argument;
        argumentTD = v.value<QDBusArgument>();
        argumentTD >> sdca;

        ULOG->log("Receive : %d", (int) sdca.blockcounter);

            if ( getSwdlId() == SWDLID::IBOX){
             timer->start(10000);
            }





        if ((sdca.blockcounter + 1 == allBlockCount) && (getSwdlId() == SWDLID::IBOX) )
            timer->stop();

	if ( sdca.status == STATUS_RESULTACK){
		ULOG->log("MOST block transfer ACK : %d / %d", (int) sdca.blockcounter + 1, 
													allBlockCount);
		if ( sdca.blockcounter + 1 == allBlockCount){
			requestTransferExit();
		}
		else{
			emit transfered( (quint64) ++m_currentBlockCount, m_allBlockCount);
			if ( getSwdlId() == SWDLID::AMP )
				transferBlock();
		}
	}
	else if ( sdca.status == STATUS_PROCESSINGACK){
		ULOG->log("TransferData Processing Ack");
	}
	else{
		ULOG_EXCEPTION;
		ULOG->log("TransferData Error");
		emit statusChanged(SWDLID::TransferData, SWDLID::Send, SWDLID::StatusError);
	}
}
void MscFileTransfer::TimeoutRequestTransferExit(){
    ULOG->log("TimeoutRequestTransferExit");
     requestTransferExit();
}

void MscFileTransfer::requestTransferExit(){
	struct SRequestTransferExit ste;

        if(getSwdlId() == SWDLID::IBOX)
          timer->stop(); 

	QDBusVariant variant;
	ste.status = STATUS_STARTRESULTACK;
	ste.swdlid = getSwdlId();
	ste.DeviceID = 1;
	ste.senderhandle = 1;
	variant.setVariant(QVariant::fromValue(ste));
	mscTrigger(SWDLID::RequestTransferExit);
	getProxy()->SetRequestTransferExit(variant);
}

void MscFileTransfer::startPrepareFileTransferPassive(){
	struct SFileInfoforSWDL sis;
	QDBusVariant variant;

	m_allBlockCount = gf_get_all_blocksize(g_ibox_file_checksum, MscAbstract::Modules()[0]->blocksize);

	sis.fileSize = m_files->fileSize;
	sis.fileName = QFileInfo(m_files->fileName).fileName();
	sis.blocksize = MscAbstract::Modules()[0]->blocksize;
	sis.filePath = m_files->fileName;
	sis.transfertype = MscAbstract::Modules()[0]->swdlchannel;	
	ULOG->log("PrepareFileTransfer IBOX : file(%s), name(%s), blocksize(%d), transfertype(%02X), size(%d)", qPrintable(sis.filePath), qPrintable(sis.fileName), sis.blocksize, (unsigned char)sis.transfertype, sis.fileSize);

	variant.setVariant(QVariant::fromValue(sis));
	QDBusPendingReply<bool> reply = getProxy()->SetPrepareFileTransferIBOX(variant);
	reply.waitForFinished();
	if (!reply.isError()){
                timer->start(10000);
		emit statusChanged(SWDLID::PrepareFileTransfer, SWDLID::Send);
	}
	else{
		emit statusChanged(SWDLID::PrepareFileTransfer, SWDLID::Send, SWDLID::StatusError, QString("startPrepareFileTransferPassive reply error"));
		disconnect(getProxy(), SIGNAL(TransferDataAMS(const QDBusVariant &)), this, 0);
	}
}

void MscFileTransfer::startPrepareFileTransfer(){
	struct SPrepareFileTransfer sft;
	QDBusVariant variant;

	m_allBlockCount = gf_get_all_blocksize(g_amplifier_file_checksum, MscAbstract::Modules()[0]->blocksize);

	sft.status = STATUS_STARTRESULTACK;
	sft.swdlid = getSwdlId();
	sft.DeviceID = DEVICE_ID;
	sft.senderhandle = SENDER_HANDLE;
	sft.filesize = m_files->fileSize;
	sft.swdlFilename = m_files->fileName;
	ULOG->log("FileTransfer : %s", qPrintable(sft.swdlFilename));
	variant.setVariant(QVariant::fromValue(sft));
	mscTrigger(SWDLID::PrepareFileTransfer);
	getProxy()->SetPrepareFileTransfer(variant);
}

void MscFileTransfer::sendCheckActivateModule(){
	struct SCheckActivateModule sam;
	QDBusVariant variant;
	sam.status = STATUS_STARTRESULTACK;
	sam.swdlid = getSwdlId();
	sam.DeviceID = DEVICE_ID;
	sam.senderhandle = SENDER_HANDLE;
	sam.checksum.append((const char *)m_files->md5sum, MD5_DIGEST_LENGTH);
	qDebug() << "Send Checksum Length : " << sam.checksum.count() << ", MD5_DIGEST_LEN : " << MD5_DIGEST_LENGTH;
	variant.setVariant(QVariant::fromValue(sam));
	mscTrigger(SWDLID::CheckActivateModule);
	getProxy()->SetCheckActivateModule(variant);
}
QDBusArgument argumentFT;
void MscFileTransfer::PrepareFileTransfer(const QDBusVariant & var){
	struct SPrepareFileTransfer sft;
	QVariant v = var.variant();
        //QDBusArgument argument;
        argumentFT = v.value<QDBusArgument>();
        argumentFT >> sft;

	if ( sft.status == STATUS_RESULTACK){	
		QString d = "filesize : " + QString::number(sft.filesize) + ", filename : " + sft.swdlFilename;
		emit statusChanged(SWDLID::PrepareFileTransfer, SWDLID::Receive, SWDLID::StatusSuccess, d);

		if ( getSwdlId() == SWDLID::AMP )
			startTransferData();
	}
	else if ( sft.status == STATUS_ERROR){
		emit statusChanged(SWDLID::PrepareFileTransfer, SWDLID::Receive, SWDLID::StatusError);
	}
}
QDBusArgument argumentTE;
void MscFileTransfer::RequestTransferExit(const QDBusVariant & var){
	struct SRequestTransferExit ste;
	QVariant v = var.variant();

        //QDBusArgument argument;
        argumentTE = v.value<QDBusArgument>();
        argumentTE >> ste;

        if (retry_ibox == 1 && (disconnect_count>0 && disconnect_count<4)){
            ::sleep(1);
               emit statusChanged(SWDLID::RequestTransferExit, SWDLID::Receive, SWDLID::StatusSuccess);
               ::sleep(1);
               startPrepareFileTransferPassive();
			   retry_ibox = 0;
        }
        else if ( ste.status == STATUS_RESULTACK){
		emit statusChanged(SWDLID::RequestTransferExit, SWDLID::Receive, SWDLID::StatusSuccess);
		sendCheckActivateModule();
	}
	else if ( ste.status == STATUS_ERROR){
		emit statusChanged(SWDLID::RequestTransferExit, SWDLID::Receive, SWDLID::StatusError);
	}
}
QDBusArgument argumentAE;
void MscFileTransfer::CheckActivateModule(const QDBusVariant & var){
	struct SCheckActivateModule sam;
	QVariant v = var.variant();
        //QDBusArgument argument;
        argumentAE = v.value<QDBusArgument>();
        argumentAE >> sam;

	if ( sam.status == STATUS_RESULTACK){
		emit statusChanged(SWDLID::CheckActivateModule, SWDLID::Receive, SWDLID::StatusSuccess);
		if ( nextFile()){
			m_files = nextFile();
			if ( getSwdlId() == SWDLID::IBOX)
				startPrepareFileTransferPassive();
			else
				startPrepareFileTransfer();
		}
		else{
                    disconnect(getProxy(), SIGNAL(iBoxSWDLFBlockVisible(const bool)), this, 0);
			startNext();
		}	
	}
	else if ( sam.status == STATUS_ERROR){
		emit statusChanged(SWDLID::CheckActivateModule, SWDLID::Receive, SWDLID::StatusError);
	}
}

void MscFileTransfer::startTransferData(){
	m_currentFile.close();
	m_currentFile.setFileName(m_files->fileName);
	m_transferSource.status = STATUS_STARTRESULTACK;
	m_transferSource.swdlid = getSwdlId();
	m_transferSource.DeviceID = DEVICE_ID;
	m_transferSource.senderhandle = SENDER_HANDLE;
	m_transferSource.blockcounter = 0;

	if ( m_currentFile.open(QIODevice::ReadOnly)){
		transferBlock();
	}
	else{
		ULOG_EXCEPTION; ULOG->log("Cannot find file");
		emit statusChanged(SWDLID::TransferData, SWDLID::Send, SWDLID::StatusError);
	}
}

void MscFileTransfer::transferBlock(){
	char buf[MscAbstract::Modules()[0]->blocksize];	
	quint64 readn = m_currentFile.read(buf, MscAbstract::Modules()[0]->blocksize);
	if ( readn > 0){
		QDBusVariant var;	
		m_transferSource.swdldata.clear();
		m_transferSource.swdldata.append(buf, readn);
		var.setVariant(QVariant::fromValue(m_transferSource));
		getProxy()->SetTransferDataAMS(var);
		qDebug() << "MOST Block transfer " << readn << ", " << m_transferSource.blockcounter + 1 << ", " << m_allBlockCount;
		m_transferSource.blockcounter++;
	}
	else{
		emit statusChanged(SWDLID::TransferData, SWDLID::Send, SWDLID::StatusError);
	}
}

void MscFileTransfer::iBoxBlockVisibleSlot(const bool state){
    retry_ibox = state;
     ULOG->log("### jaewon.yang iBoxBlockVisibleSlot [%d] getSwdlId()[%d] ###",retry_ibox,getSwdlId());
	 if ( getSwdlId() == SWDLID::IBOX){
		 if (retry_ibox == 1 && (disconnect_count>0 && disconnect_count<4)){
			 ::sleep(1);
			 requestTransferExit();
			 ULOG->log("### jaewon.yang iBoxBlockVisibleSlot retry###");
		 }
		 else if(retry_ibox == 0){
			 disconnect_count++;
			 
			 if(disconnect_count >= 4){
				 emit timeout();
				 ULOG->log("MOST disconnection count is %d", disconnect_count);
			 }
		 }
	 }

   ULOG->log("### jaewon.yang iBoxBlockVisibleSlot [%d] disconnect_count[%d]###",state,disconnect_count);
}
