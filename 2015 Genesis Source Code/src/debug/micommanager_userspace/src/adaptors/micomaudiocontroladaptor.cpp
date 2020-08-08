#include <QtGlobal>
#include <QDebug>
#include <QList>
#include <QVariant>
#include <QDBusConnection>
#include "micomaudiocontroladaptor.h"
#include "msg_processor.h"

MicomAudioControlAdaptor::MicomAudioControlAdaptor( QObject *parent ) :QDBusAbstractAdaptor(parent){}

void MicomAudioControlAdaptor::ProcessAudioControlMessage(const QDBusMessage& msg )
{
    CMsgProcessor::instance()->processDBusCall(msg);
}
