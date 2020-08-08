#include "DHAVN_AppIBox_CANMessageSender.h"
#include "DHAVN_AppIBox_LogUtils.h"

CANMessageSender::CANMessageSender(Logger &pLogger, QObject *parent) :
    m_pLogger(pLogger),
    QObject(parent)
{
    m_pQCanController = new CQCANController_ApplnTxRx(this);
    m_pQCanController->Init();
}

void CANMessageSender::send(CANMessage *pMessage)
{
//    QString dbgStr;
//    LOG(QString("pMessasge: id=[%1], dlc=[%2], data=[%3]").
//        arg(pMessage->getFrame().uqcanid).
//        arg(pMessage->getFrame().uqcandlc).
//        arg(dbgStr.setNum(qFromBigEndian<quint64>((const uchar *)pMessage->getFrame().uqcandata), 2)));

    emit Can_data_send((const SQCanFrame &)pMessage->getFrame());
}

