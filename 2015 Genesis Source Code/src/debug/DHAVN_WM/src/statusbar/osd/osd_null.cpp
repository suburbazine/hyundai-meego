#include "osd_null.h"

OSDNull::OSDNull(QPixmap *p, int variant):
AbstractOSDDraw(p, variant){
}

bool OSDNull::processCommand(const QStringList & args){
	Q_UNUSED(args);
	return false;
}

int OSDNull::getMinimumArgCount(){
	return -1;
}

const QStringList & OSDNull::getCommand(){
	return command;
}

void OSDNull::setUIPosition(){

}
