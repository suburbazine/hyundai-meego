#ifndef DHAVN_APPIBOX_CANMESSAGE_H
#define DHAVN_APPIBOX_CANMESSAGE_H

#include <QObject>

#include <DHAVN_QCANController_ApplnTxRx.h>
#include "DHAVN_AppIBox_LogUtils.h"

struct CanMessageFieldType {
    uint startByte;
    uint startBit;
    uint bitSize;
};

class CANMessage : public QObject
{
    Q_OBJECT
public:
    explicit CANMessage(Logger &pLogger, QObject *parent = 0);
    ~CANMessage();

    void setId(uint id);
    void setLengh(uchar length);
    void setData(uint startByte, uint startBit, uint bitSize, quint64 data);
    void setData(CanMessageFieldType field, quint64 data);
    SQCanFrame getFrame();

signals:

public slots:

protected:
    SQCanFrame m_frame;

private:
    DEFINE_LOGGER

};

#endif // DHAVN_APPIBOX_CANMESSAGE_H
