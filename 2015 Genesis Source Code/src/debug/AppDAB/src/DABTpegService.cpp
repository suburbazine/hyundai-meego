#include "DABTpegService.h"
#include <QDebug>

DABTpegService::DABTpegService(QObject *parent) :
    QObject(parent)
{
//    m_time.restart();
}


void
DABTpegService::onSendTPEGData(const QByteArray &data, int length)
{
    Q_UNUSED(length);
    emit sendTPEGData(data, data.size());
}
