#include "DHAVN_AppIBox_CANMessage.h"
#include "DHAVN_AppIBox_LogUtils.h"

CANMessage::CANMessage(Logger &pLogger, QObject *parent) :
    m_pLogger(pLogger),
    QObject(parent)
{
    memset(&m_frame, 0, sizeof(SQCanFrame));
}

CANMessage::~CANMessage()
{
}

void CANMessage::setId(uint id)
{
    m_frame.uqcanid = id;
}

void CANMessage::setLengh(uchar length)
{
    m_frame.uqcandlc = length;
}

#define FULL64BITS                                  (0xffffffffffffffff)
#define FULL_BITS(bitSize)                          (FULL64BITS >> (64 - bitSize))
#define MOVE_BITS(startByte, startBit, bitSize)     (FULL_BITS(bitSize) << ((7 - startByte) * 8 + startBit))

void CANMessage::setData(uint startByte, uint startBit, uint bitSize, quint64 data)
{
    QString dbgStr;
//    LOG(QString("startByte=[%1], startBit=[%2], bitSize=[%3], data=[%4]").
//        arg(startByte).arg(startBit).arg(bitSize).arg(dbgStr.setNum(data, 16)));

    quint64 existingData = qFromBigEndian<quint64>((const uchar *)m_frame.uqcandata);
    //LOG(QString("(origin) existingData=[%1]").arg(dbgStr.setNum(existingData, 16)));

    // Clean up
    quint64 moveBits = MOVE_BITS(startByte, startBit, bitSize);
    quint64 xoredBits = moveBits ^ FULL64BITS;
    existingData &= xoredBits;

    // Fill data
    existingData |= (FULL_BITS(bitSize) & data) << ((7 - startByte) * 8 + startBit);

    //LOG(QString("(update) existingData=[%1]").arg(dbgStr.setNum(existingData, 16)));

    // To BigEndian
    existingData = qToBigEndian<quint64>(existingData);

    // Copy data
    memcpy(m_frame.uqcandata, &existingData, sizeof(quint64));
}

void CANMessage::setData(CanMessageFieldType field, quint64 data)
{
    setData(field.startByte, field.startBit, field.bitSize, data);
}

SQCanFrame CANMessage::getFrame()
{
    return m_frame;
}


