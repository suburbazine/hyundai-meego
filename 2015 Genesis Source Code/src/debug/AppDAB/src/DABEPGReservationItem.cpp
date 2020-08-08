#include "DABEPGReservationItem.h"
#include "DABProtocol.h"
#include <QTextStream>
#include <QDebug>

DABEPGReservationItem::DABEPGReservationItem(QObject *parent) :
    QObject(parent)
{
    washOffData();
}

void
DABEPGReservationItem::washOffData(void)
{
    this->hour = 0;
    this->minute = 0;
    this->second = 0;
    this->duration = 0;
    this->startTime.setHMS(0,0,0);
    this->programTitle = "";
    this->description = "";
    this->serviceID = 0;
    this->mjd = 0;

    this->m_iFrequency = 0;
    this->m_iServiceType = 0;
    this->m_iSubChId = 0;
    this->m_iBitrate = 0;
    this->m_iSubCHSize = 0;
    this->m_iAddress = 0;
    this->m_sLabel = "";

    this->m_sEnsembleLabel = "";
    this->m_iPTY = 255;
    this->m_iPresetIndex = -1;

    memset(&m_xASUInfo, 0x00, sizeof(m_xASUInfo));
    memset(&m_xPICodeInfo, 0x00, sizeof(m_xPICodeInfo));
}

void
DABEPGReservationItem::setTime(int h, int m, int s, int d)
{
    this->setHour(h);
    this->setMinute(m);
    this->setSecond(s);
    this->setDuration(d);
    this->startTime.setHMS(h,m,s);
}

void
DABEPGReservationItem::saveData(stEVT_EPG_RESERVE_ITEM *pData)
{
    pData->hour = this->hour;
    pData->minute = this->minute;
    pData->second = this->second;
    pData->duration = this->duration;

    QByteArray title = this->programTitle.toUtf8();
    snprintf(pData->programTitle, sizeof(pData->programTitle), "%s", title.data());

    QByteArray desc = this->description.toUtf8();
    snprintf(pData->description, sizeof(pData->description), "%s", desc.data());

    pData->mjd = this->mjd;
    pData->serviceID = this->serviceID;

    pData->frequency = this->getFrequency();
    pData->serviceType = this->getServiceType();
    pData->subChId = this->getSubChId();
    pData->bitrate = this->getBitrate();
    pData->address = this->getAddress();
    pData->subCHSize = this->getSubCHSize();

    QByteArray label = this->getLabel().toUtf8();
    snprintf(pData->sLabel, sizeof(pData->sLabel), "%s", label.data());

    QByteArray sEnsemblelabel = this->getEnsembleLabel().toUtf8();
    snprintf(pData->sEnsembleLabel, sizeof(pData->sEnsembleLabel), "%s", sEnsemblelabel.data());

    pData->pty = this->getPTY();
    pData->presetIndex = this->getPresetIndex();



    pData->picodeCount = this->m_xPICodeInfo.PICodeCount;
    pData->picodeValidation = this->m_xPICodeInfo.PICodeValidation;
    memcpy((quint8*)pData->picodeType, (quint8*)this->m_xPICodeInfo.PICodeType, sizeof(this->m_xPICodeInfo.PICodeType));
    memcpy((quint16*)pData->picodeValue, (quint16*)this->m_xPICodeInfo.PICodeValue, sizeof(this->m_xPICodeInfo.PICodeValue));
    pData->asuFlags = this->m_xASUInfo.ASUFlags;
    pData->asuClusterCount = this->m_xASUInfo.ASUClusterCount;
    memcpy((quint8*)pData->asuClusterIDs, (quint8*)this->m_xASUInfo.ASUClusterIDs, sizeof(this->m_xASUInfo.ASUClusterIDs));
}

DABEPGReservationItem &
DABEPGReservationItem::operator =(DABEPGReservationItem& a)
{
    qDebug() << Q_FUNC_INFO;

    this->setTime(a.getHour(), a.getMinute(), a.getSecond(), a.getDuration());
    this->setProgramTitle(a.getProgramtitle());
    this->setDescription(a.getDescription());
    this->setServiceID(a.getServiceID());
    this->setMJD(a.getMJD());
    this->setFrequency(a.getFrequency());
    this->setServiceType(a.getServiceType());
    this->setSubChId(a.getSubChId());
    this->setBitrate(a.getBitrate());
    this->setAddress(a.getAddress());
    this->setSubCHSize(a.getSubCHSize());
    this->setLabel(a.getLabel());
    this->setPTY(a.getPTY());
    this->setEnsembleLabel(a.getEnsembleLabel());
    this->setPrsetIndex(a.getPresetIndex());

    m_xPICodeInfo.PICodeCount = a.m_xPICodeInfo.PICodeCount;
    m_xPICodeInfo.PICodeValidation = a.m_xPICodeInfo.PICodeValidation;
    memcpy((quint8*)m_xPICodeInfo.PICodeType, (quint8*)a.m_xPICodeInfo.PICodeType, sizeof(a.m_xPICodeInfo.PICodeType));
    memcpy((quint16*)m_xPICodeInfo.PICodeValue, (quint16*)a.m_xPICodeInfo.PICodeValue, sizeof(a.m_xPICodeInfo.PICodeValue));
    m_xASUInfo.ASUFlags = a.m_xASUInfo.ASUFlags;
    m_xASUInfo.ASUClusterCount = a.m_xASUInfo.ASUClusterCount;
    memcpy((quint8*)m_xASUInfo.ASUClusterIDs, (quint8*)a.m_xASUInfo.ASUClusterIDs, sizeof(a.m_xASUInfo.ASUClusterIDs));

    return *this;
}

void
DABEPGReservationItem::setSupplementData(DABChannelItem* pItem)
{
    qDebug() << Q_FUNC_INFO << " : freq = " << pItem->frequency();
    this->setFrequency(pItem->frequency());
    this->setServiceType(pItem->serviceType());
    this->setSubChId(pItem->subChId());
    this->setBitrate(pItem->bitrate());
    this->setAddress(pItem->address());
    this->setSubCHSize(pItem->subCHSize());
    this->setLabel(pItem->label());
    this->setPTY(pItem->programeType());
    this->setEnsembleLabel(pItem->ensembleLabel());
    this->setPrsetIndex(pItem->presetIndex());

    memset(&m_xASUInfo, 0x00, sizeof(m_xASUInfo));
    m_xASUInfo.ASUFlags = pItem->asuInfo().ASUFlags;
    m_xASUInfo.ASUClusterCount = pItem->asuInfo().ASUClusterCount;
    memcpy((quint8*)m_xASUInfo.ASUClusterIDs, (quint8*)pItem->asuInfo().ASUClusterIDs, sizeof(pItem->asuInfo().ASUClusterIDs));

    memset(&m_xPICodeInfo, 0x00, sizeof(m_xPICodeInfo));
    m_xPICodeInfo.PICodeCount = pItem->picodeInfo().PICodeCount;
    m_xPICodeInfo.PICodeValidation = pItem->picodeInfo().PICodeValidation;
    memcpy((quint8*)m_xPICodeInfo.PICodeType, (quint8*)pItem->picodeInfo().PICodeType, sizeof(pItem->picodeInfo().PICodeType));
    memcpy((quint16*)m_xPICodeInfo.PICodeValue, (quint16*)pItem->picodeInfo().PICodeValue, sizeof(pItem->picodeInfo().PICodeValue));
}

void
DABEPGReservationItem::setSupplementData(stEVT_EPG_RESERVE_ITEM *pData)
{
    this->setFrequency(pData->frequency);
    this->setServiceType(pData->serviceType);
    this->setSubChId(pData->subChId);
    this->setBitrate(pData->bitrate);
    this->setAddress(pData->address);
    this->setSubCHSize(pData->subCHSize);
    this->setLabel(QString::fromUtf8(pData->sLabel, strlen(pData->sLabel)));
    this->setEnsembleLabel(QString::fromUtf8(pData->sEnsembleLabel, strlen(pData->sEnsembleLabel)));
    this->setPTY(pData->pty);
    this->setPrsetIndex(pData->presetIndex);

    memset(&m_xASUInfo, 0x00, sizeof(m_xASUInfo));
    m_xASUInfo.ASUFlags = pData->asuFlags;
    m_xASUInfo.ASUClusterCount = pData->asuClusterCount;
    memcpy((quint8*)m_xASUInfo.ASUClusterIDs, (quint8*)pData->asuClusterIDs, sizeof(pData->asuClusterIDs));

    memset(&m_xPICodeInfo, 0x00, sizeof(m_xPICodeInfo));
    m_xPICodeInfo.PICodeCount = pData->picodeCount;
    m_xPICodeInfo.PICodeValidation = pData->picodeValidation;
    memcpy((quint8*)m_xPICodeInfo.PICodeType, (quint8*)pData->picodeType, sizeof(pData->picodeType));
    memcpy((quint16*)m_xPICodeInfo.PICodeValue, (quint16*)pData->picodeValue, sizeof(pData->picodeValue));
}
