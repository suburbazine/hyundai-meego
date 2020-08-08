/*
 * micomradiocontroladaptor.cpp
 *
 *  Created on: 2011. 9. 3.
 *      Author: yongwoo1.park
 */

#include <QtGlobal>
#include <QDebug>
#include <QList>
#include <QVariant>
#include <QDBusConnection>
#include "micomradiocontroladaptor.h"
#include "msg_processor.h"

MicomRadioControlAdaptor::MicomRadioControlAdaptor( QObject *parent ) :QDBusAbstractAdaptor(parent){}

void MicomRadioControlAdaptor::ProcessRadioControlMessage(const QDBusMessage& msg )
{
    CMsgProcessor::instance()->processDBusCall(msg);
}



