#ifndef DABTPEGSERVICE_H
#define DABTPEGSERVICE_H

#include <QObject>
#include <QTime>

class DABTpegService : public QObject
{
    Q_OBJECT
public:
    explicit DABTpegService(QObject *parent = 0);
    QTime m_time;

signals:
    void sendTPEGData(const QByteArray &tpegData, int length);
    void reqTPEGDataOff();
    void reqTPEGDataOn();

public slots:
    void onSendTPEGData(const QByteArray &data, int length);
};

#endif // DABTPEGSERVICE_H
