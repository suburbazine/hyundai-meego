#ifndef DHAVN_APPIBOX_CANMESSAGESENDER_H
#define DHAVN_APPIBOX_CANMESSAGESENDER_H

#include <QObject>

#include <DHAVN_QCANController_ApplnTxRx.h>

#include "DHAVN_AppIBox_CANMessage.h"
#include "DHAVN_AppIBox_LogUtils.h"

class CANMessageSender : public QObject
{
    Q_OBJECT
public:
    explicit CANMessageSender(Logger &pLogger, QObject *parent = 0);

    void send(CANMessage *pMessage);

signals:
    void Can_data_send(const SQCanFrame &canframe);

public slots:

private:
    DEFINE_LOGGER
    CQCANController_ApplnTxRx* m_pQCanController;

};

#endif // DHAVN_APPIBOX_CANMESSAGESENDER_H
