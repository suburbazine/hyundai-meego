#include "stdinprocess.h"

StdinProcess * StdinProcess::instance = NULL;

StdinProcess::StdinProcess(StdinProcess::RunMode m)
: QObject(NULL), mode(m), m_rs(NULL){
	ULOG->log("StdinProcess constructed : %s", qPrintable(objectName()));
}

StdinProcess::~StdinProcess(){
	emit processFinished(objectName());
	ULOG->log("StdinProcess destroyed : %s", qPrintable(objectName()));
}

bool StdinProcess::execute(const QString & m_cmd){
	ReadStdin * rs = new ReadStdin(m_cmd, this);
	if ( mode == NewInstance)
		m_rs = rs;

	connect(rs, SIGNAL(cmdLineIncome(const QString &)), SIGNAL(output(const QString &)));
	connect(rs, SIGNAL(finished()), rs, SLOT(deleteLater()));
	connect(rs, SIGNAL(finished()), SIGNAL(doFinished()));
	rs->start();
	return true;
}
