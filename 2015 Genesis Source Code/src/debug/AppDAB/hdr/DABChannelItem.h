#ifndef DABCHANNELITEM_H
#define DABCHANNELITEM_H
/**
 * FileName: DABChannelItem.h
 * Author: David.Bae
 * Time: 2011-08-43 15:24
 * Description:
 *          This file is used for Channel Data.
 */
#include <QObject>
#include <QTextStream>
#include "ListModel.h"
#include "DABProtocol.h"
#include "DABDataFrequencyTable.h"
#include "DABEPGReservationItem.h"

class DABEPGReservationItem;

class DABChannelItem : public ListItem
{
    Q_OBJECT
public:
    enum Roles { //this list is defined in DAB_HOST_IF_SPEC_VER[0.4].pdf
        FrequencyRole = Qt::UserRole+1, //size:4
        FrequencyLabelRole, //size:4
        EnsembleIDRole,     //size:2
        InternationalTableIDRole, //size:1  //v1.0
        EnsembleLabelRole,  //size:16
        ServiceCountRole,   //size:1

        ServiceIDRole,      //size:4
        ServiceTypeRole,    //size:1
        SubChIdRole,        //size:1
        BitrateRole,        //size:2
        PSRole,             //size:1
        CharsetRole,        //size:1
        ProgrammeTypeRole,//size:1    //v1.0
        ProgrammeTypeNameRole,
        LanguageRole,           //size:1    //v1.0
        LabelRole,          //size16
        IsDABtoDABRole,
        IsDABtoFMRole,
        IsNoSignalRole,			
        PresetIndexRole,
        LockRole,
        IsSkippedRole
    };

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

    //explicit DABChannelItem(stRSP_GET_SERVICE_LIST* event, QObject *parent = 0);
    DABChannelItem( QObject *parent=0);
    DABChannelItem(DABChannelItem* a, QObject *parent = 0);
    explicit DABChannelItem(stRSP_GET_ENSEMBLE_LIST* ensemble, stRSP_GET_SERVICE_LIST* service, QObject *parent = 0);
    QVariant data(int role) const;
    QHash<int, QByteArray> roleNames() const;

    inline QString id() const { return QString("%1").arg(m_iServiceID); }
    inline quint32 key() const { return m_iFrequency; }
    inline quint8 subKey() const { return m_iSubChId; }
    inline quint8 ausFlag() const { return m_xASUInfo.ASUFlags; }
    inline QString ensemble() const { return m_sEnsembleLabel; }
    inline quint32 frequency() const { return m_iFrequency;}
    inline QString frequencyLabel() const { return m_sFrequencyLabel;}
    inline quint16 ensembleID() const { return m_iEnsembleID;}
    inline quint8  internationalTableID() const {return this->m_iInternationalTableID;} //v1.0
    inline QString ensembleLabel() const { return m_sEnsembleLabel;}
    inline quint8  serviceCount() const { return m_iServiceCount; }

    inline quint32 serviceID() const { return m_iServiceID;}
    inline quint8  serviceType() const { return m_iServiceType; }
    inline quint8  subChId() const { return m_iSubChId; }
    inline quint16 bitrate() const { return m_iBitrate;}
    inline quint8  ps() const { return m_iPS; }
    inline quint8  charset() const { return m_iCharset; }
    inline quint8  programeType() const {return this->m_iProgrameType;}   //v1.0
    inline QString programeTypeName() const {return this->m_sProgrameTypeName;}
    inline quint8  language() const {return this->m_iLanguage;}                         //v1.0
    inline QString label() const { return m_sLabel;}
    inline bool    isDABtoDAB() const {return m_bIsDABtoDAB; }
    inline bool    isDABtoFM() const {return m_bIsDABtoFM; }
    inline bool    isNoSignal() const {return m_bIsNoSignal; }
    inline int     presetIndex() const { return m_iPresetIndex; }
    inline int     lock() const { return m_bLock; }
    inline bool    isSkipped() const {return m_bIsSkipped; }
    inline quint8  subCHSize() const { return m_iSubCHSize; }
    inline quint16 address() const { return m_iAddress; }
    inline PICodeInfo picodeInfo() const { return m_xPICodeInfo; }
    inline ASUInfo asuInfo() const { return m_xASUInfo; }

    //QTextStream &operator>>(QChar &ch);
    bool loadData(QTextStream &in);
    bool saveData(QTextStream &out);
    void setASUInfo(stASU_INFO* asuInfo);
    void setPICodeInfo(stPICODE_INFO* picodeInfo);
    inline void setFrequency(quint32 f) {this->m_iFrequency = f; m_sFrequencyLabel = DABDataFrequencyTable::GetLabel(m_iFrequency);}
    inline void setFrequencyLabel(QString str) {this->m_sFrequencyLabel = str; }
    inline void setEnsembleID(quint16 eId) {this->m_iEnsembleID = eId;}
    inline void setInternationalTableID(quint8 id) { this->m_iInternationalTableID = id;} //v1.0
    inline void setEnsembleLabel(QString str) {this->m_sEnsembleLabel = str;}
    inline void setServiceCount(quint8 count) {this->m_iServiceCount = count;}
    inline void setServiceID(quint32 sID) {this->m_iServiceID = sID;}
    inline void setServiceType(quint8 sType) {this->m_iServiceType = sType;}
    inline void setSubChId(quint8 subId) {this->m_iSubChId = subId;}
    inline void setBitrate(quint16 bit) {this->m_iBitrate = bit;}
    inline void setPS(quint8 ps) {this->m_iPS = ps;}
    inline void setCharset(quint8 charset) {this->m_iCharset = charset;}
    inline void setProgrameType(quint8 pty) {this->m_iProgrameType = pty;}    //v1.0
    inline void setProgrameTypeName(QString ptyName) {this->m_sProgrameTypeName = ptyName;}
    inline void setLanguage(quint8 lang) {this->m_iLanguage = lang;}                        //v1.0
    inline void setLabel(QString label) {this->m_sLabel = label;}
    inline void setDABtoDAB(bool onOff) { this->m_bIsDABtoDAB = onOff; }
    inline void setDABtoFM(bool onOff) { this->m_bIsDABtoFM = onOff; }
    inline void setNoSignal(bool onOff) { this->m_bIsNoSignal = onOff; }
    inline void setPresetIndex(int index) { this->m_iPresetIndex = index; }
    inline void setLock(bool lock) { this->m_bLock = lock; }
    inline void setSkip(bool skipOn) { this->m_bIsSkipped = skipOn; }
    inline void setSubCHSize(quint8 size) { this->m_iSubCHSize = size; }
    inline void setAddress(quint16 address) { this->m_iAddress = address; }
    inline void setSkipOn(void) { setSkip(true); }
    inline void setSkipOff(void) { setSkip(false); }
    void setSupplementary(ASUInfo asuInfo, PICodeInfo picodeInfo);
    void setLinkingInfo(DABEPGReservationItem* pItem);
    void clearLiningInfo(void);
    DABChannelItem & operator =(DABChannelItem& a);

    void washOffData(void);
signals:

public slots:

private:
    //Ensemble
    quint32 m_iFrequency;
    QString m_sFrequencyLabel;
    quint16 m_iEnsembleID;
    quint8  m_iInternationalTableID;    //v1.0
    QString m_sEnsembleLabel;
    quint8  m_iServiceCount;

    //Service
    quint32 m_iServiceID;
    quint16 m_iCER;
    quint8  m_iServiceType;
    quint8  m_iSubChId;
    quint16 m_iBitrate;
    quint8  m_iPS;
    quint8  m_iCharset;
    quint8  m_iProgrameType;     //v1.0
    QString m_sProgrameTypeName;
    quint8  m_iLanguage;                //v1.0
    QString m_sLabel;
    bool    m_bIsDABtoDAB;
    bool    m_bIsDABtoFM;
    bool    m_bIsNoSignal;
    int     m_iPresetIndex;
    bool    m_bLock;
    bool    m_bIsSkipped;               //for List Menu
    quint8  m_iSubCHSize;
    quint16 m_iAddress;

    ASUInfo m_xASUInfo;
    PICodeInfo m_xPICodeInfo;
};
#endif // DABCHANNELITEM_H
