#include "osdwriter.h"
#include "messageprocess.h"
#include <QCoreApplication>
#include <QFileInfo>

void OSDWriter::SendOSD(const QString & msg){
	QFileInfo info(QCoreApplication::applicationFilePath());
	MessageProcess::instance(info.fileName())->writeMessage(msg);
}
