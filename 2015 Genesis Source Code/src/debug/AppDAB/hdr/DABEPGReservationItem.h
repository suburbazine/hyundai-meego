#ifndef DABEPGRESERVATIONITEM_H
#define DABEPGRESERVATIONITEM_H

#include <QObject>
#include <QTime>

#include "DABChannelItem.h"
#include "DABProtocol.h"

class DABChannelItem;

class DABEPGReservationItem : public QObject
{
    Q_OBJECT
public:

    struct ASUInfo {
        quint16     ASUFlags;               //size:2
        quint8      ASUClusterCount;        //size:1
        quint8      ASUClusterIDs[23];      //size:23
    };

    struct PICodeInfo {
        quint8      PICodeCount;            //size:1
        quint8      PICodeType[12];         //size:12
        quint16     PICodeValue[12];        //size:24
        quint16     PICodeValidation;       //size:2
    };

    explicit DABEPGReservationItem(QObject *parent = 0);
    void washOffData(void);

    inline int getHour(void) const { return this->hour; }
    inline int getMinute(void) const { return this->minute; }
    inline int getSecond(void) const { return this->second; }
    inline int getDuration(void) const { return this->duration; }
    inline QTime getStartTime(void) const { return this->startTime;}
    inline QString getProgramtitle(void) const { return this->programTitle;}
    inline QString getDescription(void) const { return this->description;}
    inline quint32 getServiceID() const { return this->serviceID;}
    inline quint32 getMJD() const { return this->mjd;}

    inline quint32 getFrequency() const { return m_iFrequency; }
    inline quint8  getServiceType() const { return m_iServiceType; }
    inline quint8  getSubChId() const { return m_iSubChId; }
    inline quint16 getBitrate() const { return m_iBitrate;}
    inline QString getLabel() const { return m_sLabel;}
    inline quint8  getSubCHSize() const { return m_iSubCHSize; }
    inline quint16 getAddress() const { return m_iAddress; }

    inline quint8 getPTY() const { return m_iPTY; }
    inline QString getEnsembleLabel() const { return m_sEnsembleLabel; }
    inline int getPresetIndex() const { return m_iPresetIndex; }

    inline PICodeInfo getPicodeInfo() const { return m_xPICodeInfo; }
    inline ASUInfo getAsuInfo() const { return m_xASUInfo; }

    void setHour(int h) { this->hour = h; }
    void setMinute(int m) { this->minute = m; }
    void setSecond(int s) { this->second = s; }
    void setDuration(int d) { this->duration = d; }
    void setTime(int h, int m, int s, int d);
    void setProgramTitle(QString title) { this->programTitle = title; }
    void setDescription(QString desc) { this->description = desc; }
    void setServiceID(quint32 serviceID){ this->serviceID = serviceID; }
    void setMJD(quint32 mjd){ this->mjd = mjd; }

    inline void setFrequency(quint32 freq) { this->m_iFrequency = freq; }
    inline void setServiceType(quint8 sType) { this->m_iServiceType = sType; }
    inline void setSubChId(quint8 subId) { this->m_iSubChId = subId; }
    inline void setBitrate(quint16 bit) { this->m_iBitrate = bit; }
    inline void setLabel(QString label) {this->m_sLabel = label; }
    inline void setSubCHSize(quint8 size) { this->m_iSubCHSize = size; }
    inline void setAddress(quint16 address) { this->m_iAddress = address; }

    inline void setPTY(quint8 pty) { this->m_iPTY = pty; }
    inline void setEnsembleLabel(QString label) { this->m_sEnsembleLabel = label; }
    inline void setPrsetIndex(int index) { this->m_iPresetIndex = index; }

    void saveData(stEVT_EPG_RESERVE_ITEM *pData);
    void setSupplementData(DABChannelItem* pItem);
    void setSupplementData(stEVT_EPG_RESERVE_ITEM *pData);
    DABEPGReservationItem & operator =(DABEPGReservationItem& a);

signals:

public slots:

private:
    int         hour;
    int         minute;
    int         second;
    int         duration;
    QTime       startTime;
    QString     programTitle;
    QString     description;
    quint32     serviceID;
    quint32     mjd;

    quint32     m_iFrequency;
    quint8      m_iServiceType;
    quint8      m_iSubChId;
    quint16     m_iBitrate;
    quint8      m_iSubCHSize;
    quint16     m_iAddress;
    QString     m_sLabel;
    QString     m_sEnsembleLabel;
    quint8      m_iPTY;
    int         m_iPresetIndex;
    ASUInfo     m_xASUInfo;
    PICodeInfo  m_xPICodeInfo;
};

#endif // DABEPGRESERVATIONITEM_H
