#include "checkpartition.h"
#include "stdinprocess.h"
#include "common.h"
#include "partition.h"

CheckPartition::CheckPartition(QObject * obj) : AbstractUpdater(AbstractUpdater::PartitionLauncher, obj){
	m_currentIndex = 0;
	m_stopFlag = false;
}

void CheckPartition::setUpdateList(const QList<UpdateElement *> & list){
	for (int i = 0; i < list.count(); ++i)
		if ( list.at(i)->type() == UpdateElement::et_PARTITION)
			m_targetImages.append(list.at(i));
}

void CheckPartition::start(){
	checkPartitions();
}

void CheckPartition::stop(){
	m_stopFlag = true;
}

void CheckPartition::checkPartitions(){
	if ( m_targetImages.count() == 0){
		emit finished(0);
		return;
	}

	StdinProcess * sp = StdinProcess::CreateNewInstance();
	connect(sp, SIGNAL(output(const QString &)), SLOT(processStdin(const QString &)));

	sp->execute(QString(PARTCLONE_CHKIMG_NAME) +
				PARTCLONE_SOURCE_OPTION +
				SPACE_STR(m_targetImages.at(0)->path()));

	connect(sp, SIGNAL(doFinished()), sp, SLOT(deleteLater()));
}


void CheckPartition::startNext(){
	if ( !m_stopFlag ){
		m_targetImages.takeFirst();
		m_currentIndex++;
		checkPartitions();
	}
}

void CheckPartition::parseProgress(const QString & str){
	QStringList token = str.split(",");
	for (int i = 0; i < token.count(); ++i){
		QString trimmed = token.at(i).trimmed();
		QStringList column = trimmed.split(":");
		if ( column.count() == 2){
			QString section = column.at(0).trimmed();
			QString value = column.at(1).trimmed();
			ULOG->log("checkimage(%s) section : %s, value : %s", qPrintable(m_targetImages.at(0)->name()), qPrintable(section), qPrintable(value));	
			if ( section == "Completed"){
				value = value.remove("%");
				int percentage = (int)value.toDouble();
				emit updateProgressChanged(QString("@") + m_targetImages.at(0)->name(), tr("STR_UPGRADE_TITLE_CHECK_IMAGE"), percentage, m_targetImages.at(0)->weight());
			}
		}
	}
}

void CheckPartition::processStdin(const QString & output){
	if ( output.contains("Checked successfully")){
		ULOG->log("checked successfully");
		startNext();
	}
	else if ( output.contains("Completed")){
		parseProgress(output);
	}
	else if ( output.contains("fail") || output.contains("error")){
		ULOG->log("%s", qPrintable(output));
		emit finished(1);
	}
}
