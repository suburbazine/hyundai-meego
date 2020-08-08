#include "DABRdsAdaptor.h"
#include <QDebug>
#include <QTimer>

DABRdsAdaptor::DABRdsAdaptor(QObject *parent) :
    QObject(parent)
{
    memset(&m_xAnnouncementStatus, 0x00, sizeof(m_xAnnouncementStatus));
    m_xAnnouncementStatus.eTAStatus = TA_STATUS_INVAILD;
    m_xAnnouncementStatus.bAnnouncePopup = false;
    m_bTraffic = false;
    m_bAnnouncePopupFail = false;
}

void
DABRdsAdaptor::init(void)
{
    m_xDABRdsSharedMem.setKey(RDS_SHARED_MEM_KEY);
    m_xDABRdsSharedMem.attach();
    m_xDABRdsSharedMem.create(sizeof(m_xAnnouncementStatus));
}

bool
DABRdsAdaptor::readAnnouncePopupStatus(void)
{
    m_xDABRdsSharedMem.lock();
    memcpy(&m_xAnnouncementStatus, m_xDABRdsSharedMem.data(), sizeof(m_xAnnouncementStatus));
    m_xDABRdsSharedMem.unlock();

    m_bAnnouncePopupFail = m_xAnnouncementStatus.bAnnouncePopup;
    return m_xAnnouncementStatus.bAnnouncePopup;
}

void
DABRdsAdaptor::writeAnnouncePopupStatus(bool onOff)
{
    qDebug() << Q_FUNC_INFO << ":: onOff = " << onOff;
    DABLogger::instance()->Log((QString(" writeAnnouncePopupStatus() :: RDS Announcement Popup onOff = %1").arg( onOff)));

    m_xDABRdsSharedMem.lock();
    memcpy(&m_xAnnouncementStatus, m_xDABRdsSharedMem.data(), sizeof(m_xAnnouncementStatus));
    m_xDABRdsSharedMem.unlock();

    DABLogger::instance()->Log((QString(" writeAnnouncePopupStatus() :: RDS Announcement Popup onOff = AnnouncePopup = %1, TAStatus = %2").arg(m_xAnnouncementStatus.bAnnouncePopup).arg(m_xAnnouncementStatus.eTAStatus)));
    m_xAnnouncementStatus.bAnnouncePopup = onOff;
    m_xDABRdsSharedMem.lock();
    char *to = (char*)m_xDABRdsSharedMem.data();
    memcpy(to, &m_xAnnouncementStatus, sizeof(m_xAnnouncementStatus));
    m_xDABRdsSharedMem.unlock();
    m_bAnnouncePopupFail = false;
}

void
DABRdsAdaptor::onReqTrafficDABtoRDS(bool onOff)
{
    qDebug() << Q_FUNC_INFO << ":: onOff = " << onOff << ", m_bTraffic = " << m_bTraffic;
    DABLogger::instance()->Log((QString(" onReqTrafficDABtoRDS() :: m_bTraffic = %1").arg(onOff)));
    m_bTraffic = onOff;
    emit reqTrafficDABToRDS(onOff);    
}

void
DABRdsAdaptor::onTrafficRDStoDAB(bool onOff)
{
    qDebug() << Q_FUNC_INFO << ":: onOff = " << onOff;
    m_bTraffic = onOff;
    emit cmdTrafficRDStoDAB(onOff);
}

void
DABRdsAdaptor::onCheckRDSLanuch(void)
{
    DABLogger::instance()->Log((QString(" onCheckRDSLanuch()")));
    m_xDABRdsSharedMem.lock();
    memcpy(&m_xAnnouncementStatus, m_xDABRdsSharedMem.data(), sizeof(m_xAnnouncementStatus));
    m_xDABRdsSharedMem.unlock();

    qDebug() << __FUNCTION__ << " RDS Announcement Popup onOff = " << m_xAnnouncementStatus.bAnnouncePopup << ", eTAStatus = " << m_xAnnouncementStatus.eTAStatus;
    DABLogger::instance()->Log((QString(" onCheckRDSLanuch() :: RDS Announcement Popup onOff = %1, eTAStatus = %2").arg( m_xAnnouncementStatus.bAnnouncePopup).arg(m_xAnnouncementStatus.eTAStatus)));

    if(m_xAnnouncementStatus.eTAStatus == TA_STATUS_INVAILD)
    {
        QTimer::singleShot(1000, this, SLOT(onCheckRDSLanuch()));
    }
    else
    {
        if(m_xAnnouncementStatus.eTAStatus == TA_STATUS_OFF)
        {
            emit cmdReqSetAnnounement(false);
        }
        else
        {
            emit cmdReqSetAnnounement(true);
        }
    }
}
