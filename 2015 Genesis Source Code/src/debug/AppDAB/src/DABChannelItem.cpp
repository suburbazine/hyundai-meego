/**
 * FileName: DABChannelItem.cpp
 * Author: David.Bae
 * Time: 2011-08-43 15:24
 * Description:
 *          This file is used for Channel Data.
 */

#include <QTextStream>
#include "DABChannelItem.h"
#include <QDebug>
#include "DABDataFrequencyTable.h"

DABChannelItem::DABChannelItem(stRSP_GET_ENSEMBLE_LIST* ensemble, stRSP_GET_SERVICE_LIST* service, QObject *parent) :
    ListItem(parent),
    m_iFrequency(ensemble->Frequency),
    m_iEnsembleID(ensemble->EnsembleID),
    m_iInternationalTableID(ensemble->InternationalTableID),
    m_iServiceCount(ensemble->ServiceCount),
    m_iServiceID(service->ServiceID),
    m_iServiceType(service->ServiceType),
    m_iSubChId(service->SubChId),
    m_iBitrate(service->Bitrate),
    m_iPS(service->PS),
    m_iCharset(service->Charset),
    m_iProgrameType(service->ProgrammeType),
    m_iLanguage(service->Language),
    m_iSubCHSize(service->SubCHSize),
    m_iAddress(service->Address)
{
    m_sFrequencyLabel = DABDataFrequencyTable::GetLabel(m_iFrequency);
    m_sLabel = QByteArray((const char*)service->Label);
    m_sEnsembleLabel = QByteArray((const char*)ensemble->EnsembleLabel);
    m_bIsDABtoDAB = false;
    m_bIsDABtoFM = false;
    m_bIsNoSignal = false;
    m_iPresetIndex = 0;
    m_bLock = true;
    m_bIsSkipped = false; // Skip info in List Menu
    m_iCER = 0;
    memset(&m_xASUInfo, 0x00, sizeof(m_xASUInfo));
    memset(&m_xPICodeInfo, 0x00, sizeof(m_xPICodeInfo));

}
DABChannelItem::DABChannelItem( QObject *parent):
    ListItem(parent)
{
    washOffData();
}

DABChannelItem::DABChannelItem(DABChannelItem* a, QObject *parent):
ListItem(parent)
{
    this->setFrequency(a->frequency());
    this->setFrequencyLabel(a->frequencyLabel());
    this->setEnsembleID(a->ensembleID());
    this->setInternationalTableID(a->internationalTableID());
    this->setEnsembleLabel(a->ensembleLabel());
    this->setServiceCount(a->serviceCount());

    this->setServiceID(a->serviceID());
    this->setServiceType(a->serviceType());
    this->setSubChId(a->subChId());
    this->setBitrate(a->bitrate());
    this->setPS(a->ps());
    this->setCharset(a->charset());
    this->setProgrameType(a->programeType());
    this->setProgrameTypeName(a->programeTypeName());
    this->setLanguage(a->language());
    this->setLabel(a->label());
    this->setDABtoDAB(a->isDABtoDAB());
    this->setDABtoFM(a->isDABtoFM());
    this->setNoSignal(a->isNoSignal());
    this->setPresetIndex(a->presetIndex());
    this->setSkip(a->isSkipped());
    this->setLock(a->lock());
    this->setSubCHSize(a->subCHSize());
    this->setAddress(a->address());
    m_iCER = 0;
    m_xPICodeInfo.PICodeCount = a->m_xPICodeInfo.PICodeCount;
    m_xPICodeInfo.PICodeValidation = a->m_xPICodeInfo.PICodeValidation;
    memcpy((quint8*)m_xPICodeInfo.PICodeType, (quint8*)a->m_xPICodeInfo.PICodeType, sizeof(a->m_xPICodeInfo.PICodeType));
    memcpy((quint16*)m_xPICodeInfo.PICodeValue, (quint16*)a->m_xPICodeInfo.PICodeValue, sizeof(a->m_xPICodeInfo.PICodeValue));
    m_xASUInfo.ASUFlags = a->m_xASUInfo.ASUFlags;
    m_xASUInfo.ASUClusterCount = a->m_xASUInfo.ASUClusterCount;
    memcpy((quint8*)m_xASUInfo.ASUClusterIDs, (quint8*)a->m_xASUInfo.ASUClusterIDs, sizeof(a->m_xASUInfo.ASUClusterIDs));
}

QHash<int, QByteArray>
DABChannelItem::roleNames() const
{
    QHash<int, QByteArray> names;

    names[FrequencyRole] = "frequency";
    names[FrequencyLabelRole] = "frequencyLabel";
    names[EnsembleIDRole] = "ensembleID";
    names[InternationalTableIDRole] = "internationalTableID";   //v1.0
    names[EnsembleLabelRole] = "ensembleLabel";
    names[ServiceCountRole] = "serviceCount";
    names[ServiceIDRole] = "serviceID";
    names[ServiceTypeRole] = "serviceType";
    names[SubChIdRole] = "subChId";
    names[BitrateRole] = "bitrate";
    names[PSRole] = "ps";
    names[CharsetRole] = "charset";
    names[ProgrammeTypeRole] = "programeType";     //v1.0
    names[ProgrammeTypeNameRole] = "programeTypeName";     //v1.0
    names[LanguageRole] = "language";                          //v1.0
    names[LabelRole] = "label";
    names[IsDABtoDABRole] = "isDABtoDAB";
    names[IsDABtoFMRole] = "isDABtoFM";
    names[IsNoSignalRole] = "isNoSignal";
    names[PresetIndexRole] = "presetIndex";
    names[LockRole] = "lock";
    names[IsSkippedRole] = "isSkipped";
    return names;
}

QVariant
DABChannelItem::data(int role) const
{
    switch(role) {
    case FrequencyRole:
        return frequency();
    case FrequencyLabelRole:
        return frequencyLabel();
    case EnsembleIDRole:
        return ensembleID();
    case InternationalTableIDRole:      //v1.0
        return internationalTableID();
    case EnsembleLabelRole:
        return ensembleLabel();
    case ServiceCountRole:
        return serviceCount();
    case ServiceIDRole:
        return serviceID();
    case ServiceTypeRole:
        return serviceType();
    case SubChIdRole:
        return subChId();
    case BitrateRole:
        return bitrate();
    case PSRole:
        return ps();
    case CharsetRole:
        return charset();
    case ProgrammeTypeRole:       //v1.0
        return programeType();
    case ProgrammeTypeNameRole:       //v1.0
        return programeTypeName();
    case LanguageRole:                  //v1.0
        return language();
    case LabelRole:
        return label();
    case IsDABtoDABRole:
        return isDABtoDAB();
    case IsDABtoFMRole:
        return isDABtoFM();
    case IsNoSignalRole:
        return isNoSignal();	
    case PresetIndexRole:
        return presetIndex();
    case LockRole:
        return lock();
    case IsSkippedRole:
        return isSkipped();
    default:
        return QVariant();
    }
}

bool
DABChannelItem::loadData(QTextStream &in)
{
    QString readData;
    QHash<int, QByteArray> names = this->roleNames();

    //Ensemble
    readData = in.readLine();
    if(readData.startsWith(names.value(FrequencyRole)))
    {
        readData.remove(names.value(FrequencyRole)).remove(":");
        this->m_iFrequency = readData.toUInt();
        m_sFrequencyLabel = DABDataFrequencyTable::GetLabel(m_iFrequency);
    }
    else
    {
        qDebug() << " Can't find : " << QString(names.value(FrequencyRole));
        return false;
    }

    //In C&S Spec v0.5, This Frequency Label removed.
    //this->m_sFrequencyLabel = in.readLine();

    readData = in.readLine();
    if(readData.startsWith(names.value(EnsembleIDRole))){
        //qDebug() << QString(names.value(EnsembleIDRole)) << " : ";
        readData.remove(names.value(EnsembleIDRole)).remove(":");
        this->m_iEnsembleID = readData.toUShort();
        //qDebug() << "eID: " << this->m_iEnsembleID;
    }else{
        qDebug() << " Can't find : " << QString(names.value(EnsembleIDRole));
        return false;
    }
    //InternationalTableIDRole
    readData = in.readLine();
    if(readData.startsWith(names.value(InternationalTableIDRole))){
        //qDebug() << readData;
        readData.remove(names.value(InternationalTableIDRole)).remove(":");
        this->m_iInternationalTableID = readData.toUInt();
        //qDebug() << "m_iInternationalTableID: " << this->m_iInternationalTableID;
    }else{
        qDebug() << " Can't find : " << QString(names.value(InternationalTableIDRole));
        return false;
    }
    //this->m_iEnsembleID = in.readLine().toUShort();
    readData = in.readLine();
    if(readData.startsWith(names.value(EnsembleLabelRole))){
        //qDebug() << QString(names.value(EnsembleIDRole)) << " : ";
        readData.remove(names.value(EnsembleLabelRole)).remove(":");
        this->m_sEnsembleLabel = readData;
    }else{
        qDebug() << " Can't find : " << QString(names.value(EnsembleLabelRole));
        return false;
    }
    //this->m_sEnsembleLabel = in.readLine();
    readData = in.readLine();
    if(readData.startsWith(names.value(ServiceCountRole))){
        //qDebug() << QString(names.value(ServiceCountRole)) << " : ";
        readData.remove(names.value(ServiceCountRole)).remove(":");
        this->m_iServiceCount = readData.toUInt();
    }else{
        qDebug() << " Can't find : " << QString(names.value(ServiceCountRole));
        return false;
    }
    //this->m_iServiceCount = in.readLine().toUInt();

    //Service
    readData = in.readLine();
    if(readData.startsWith(names.value(ServiceIDRole))){
        //qDebug() << QString(names.value(ServiceIDRole)) << " : ";
        readData.remove(names.value(ServiceIDRole)).remove(":");
        this->m_iServiceID = readData.toUInt();
        //qDebug() << "sID:" << this->m_iServiceID;
    }else{
        qDebug() << " Can't find : " << QString(names.value(ServiceIDRole));
        return false;
    }
    //this->m_iServiceID = in.readLine().toUInt();
    readData = in.readLine();
    if(readData.startsWith(names.value(ServiceTypeRole))){
        //qDebug() << QString(names.value(ServiceTypeRole)) << " : ";
        readData.remove(names.value(ServiceTypeRole)).remove(":");
        this->m_iServiceType = readData.toUShort();
    }else{
        qDebug() << " Can't find : " << QString(names.value(ServiceTypeRole));
        return false;
    }
    //this->m_iServiceType = in.readLine().toUShort();
    readData = in.readLine();
    if(readData.startsWith(names.value(SubChIdRole))){
        //qDebug() << QString(names.value(SubChIdRole)) << " : ";
        readData.remove(names.value(SubChIdRole)).remove(":");
        this->m_iSubChId = readData.toUShort();
    }else{
        qDebug() << " Can't find : " << QString(names.value(SubChIdRole));
        return false;
    }
    //this->m_iSubChId = in.readLine().toUShort();
    readData = in.readLine();
    if(readData.startsWith(names.value(BitrateRole))){
        //qDebug() << QString(names.value(BitrateRole)) << " : ";
        readData.remove(names.value(BitrateRole)).remove(":");
        this->m_iBitrate = readData.toUShort();
    }else{
        qDebug() << " Can't find : " << QString(names.value(BitrateRole));
        return false;
    }
    //this->m_iBitrate = in.readLine().toUShort();

    readData = in.readLine();
    if(readData.startsWith(names.value(PSRole))){
        //qDebug() << QString(names.value(CharsetRole)) << " : ";
        readData.remove(names.value(PSRole)).remove(":");
        this->m_iPS = readData.toUShort();
    }else{
        qDebug() << " Can't find : " << QString(names.value(PSRole));
        return false;
    }

    readData = in.readLine();
    if(readData.startsWith(names.value(CharsetRole))){
        //qDebug() << QString(names.value(CharsetRole)) << " : ";
        readData.remove(names.value(CharsetRole)).remove(":");
        this->m_iCharset = readData.toUShort();
    }else{
        qDebug() << " Can't find : " << QString(names.value(CharsetRole));
        return false;
    }
   // this->m_iCharset = in.readLine().toUShort();


    //quint8  m_iProgrameType;     //v1.0
    readData = in.readLine();
    if(readData.startsWith(names.value(ProgrammeTypeRole))){
        readData.remove(names.value(ProgrammeTypeRole)).remove(":");
        this->m_iProgrameType = readData.toUShort();
    }else{
        qDebug() << " Can't find : " << QString(names.value(ProgrammeTypeRole));
        return false;
    }
    //quint8  m_iLanguage;                //v1.0
    readData = in.readLine();
    if(readData.startsWith(names.value(LanguageRole))){
        readData.remove(names.value(LanguageRole)).remove(":");
        this->m_iLanguage = readData.toUShort();
    }else{
        qDebug() << " Can't find : " << QString(names.value(LanguageRole));
        return false;
    }
    readData = in.readLine();
    if(readData.startsWith(names.value(LabelRole))){
        //qDebug() << QString(names.value(CharsetRole)) << " : ";
        readData.remove(names.value(LabelRole)).remove(":");
        this->m_sLabel = readData;
    }else{
        qDebug() << " Can't find : " << QString(names.value(LabelRole));
        return false;
    }
    //this->m_sLabel = in.readLine();

    readData = in.readLine();
    if(readData.startsWith(names.value(IsDABtoDABRole))){
        //qDebug() << QString(names.value(CharsetRole)) << " : ";
        readData.remove(names.value(IsDABtoDABRole)).remove(":");
        this->m_bIsDABtoDAB = (readData.toInt()==1)?true:false;
    }else{
        qDebug() << " Can't find : " << QString(names.value(IsDABtoDABRole));
        return false;
    }

    readData = in.readLine();
    if(readData.startsWith(names.value(IsDABtoFMRole))){
        //qDebug() << QString(names.value(CharsetRole)) << " : ";
        readData.remove(names.value(IsDABtoFMRole)).remove(":");
        this->m_bIsDABtoFM = (readData.toInt()==1)?true:false;
    }else{
        qDebug() << " Can't find : " << QString(names.value(IsDABtoFMRole));
        return false;
    }

    readData = in.readLine();
    if(readData.startsWith(names.value(IsNoSignalRole))){
        //qDebug() << QString(names.value(CharsetRole)) << " : ";
        readData.remove(names.value(IsNoSignalRole)).remove(":");
        this->m_bIsNoSignal = (readData.toInt()==1)?true:false;
    }else{
        qDebug() << " Can't find : " << QString(names.value(IsNoSignalRole));
        return false;
    }

    readData = in.readLine();
    if(readData.startsWith(names.value(PresetIndexRole))){
        readData.remove(names.value(PresetIndexRole)).remove(":");
        this->m_iPresetIndex = readData.toInt();
    }else{
        qDebug() << " Can't find : " << QString(names.value(PresetIndexRole));
        return false;
    }

    readData = in.readLine();
    if(readData.startsWith(names.value(LockRole))){
        //qDebug() << QString(names.value(CharsetRole)) << " : ";
        readData.remove(names.value(LockRole)).remove(":");
        this->m_bLock = (readData.toInt()==1)?true:false;
    }else{
        qDebug() << " Can't find : " << QString(names.value(LockRole));
        return false;
    }

    readData = in.readLine();
    if(readData.startsWith("SubCHSize")){
        readData.remove("SubCHSize").remove(":");
        this->m_iSubCHSize = readData.toUShort();
    }else{
        qDebug() << " Can't find : " << QString("SubCHSize");
    }

    readData = in.readLine();
    if(readData.startsWith("Address")){
        readData.remove("Address").remove(":");
        this->m_iAddress = readData.toUShort();
    }else{
        qDebug() << " Can't find : " << QString("Address");
        return false;
    }

    readData = in.readLine();
    if(readData.startsWith(names.value(IsSkippedRole))){
        //qDebug() << QString(names.value(CharsetRole)) << " : ";
        readData.remove(names.value(IsSkippedRole)).remove(":");
        this->m_bIsSkipped = (readData.toInt()==1)?true:false;
    }else{
        qDebug() << " Can't find : " << QString(names.value(IsSkippedRole));
        return false;
    }

    readData = in.readLine();
    if(readData.startsWith("ASUFlags")){
        readData.remove("ASUFlags").remove(":");
        this->m_xASUInfo.ASUFlags = readData.toUShort();
    }else{
        qDebug() << " Can't find : " << QString("ASUFlags");
        return false;
    }

    readData = in.readLine();
    if(readData.startsWith("ASUClusterCount")){
        readData.remove("ASUClusterCount").remove(":");
        this->m_xASUInfo.ASUClusterCount = readData.toUShort();
    }else{
        qDebug() << " Can't find : " << QString("ASUClusterCount");
        return false;
    }

    if(this->m_xASUInfo.ASUClusterCount > 22)
    {
        qDebug() << " ASUClusterCount is big than 22 : " << this->m_xASUInfo.ASUClusterCount;
        return false;
    }

    for(int i=0; i<this->m_xASUInfo.ASUClusterCount; i++)
    {
        readData = in.readLine();
        if(readData.startsWith("ASUClusterIDs")){
            readData.remove("ASUClusterIDs").remove(":");
            this->m_xASUInfo.ASUClusterIDs[i] = readData.toUShort();
        }else{
            qDebug() << " Can't find : " << QString("ASUClusterIDs");
            return false;
        }
    }

    readData = in.readLine();
    if(readData.startsWith("PICodeCount")){
        readData.remove("PICodeCount").remove(":");
        this->m_xPICodeInfo.PICodeCount = readData.toUShort();
    }else{
        qDebug() << " Can't find : " << QString("PICodeCount");
        return false;
    }

    readData = in.readLine();
    if(readData.startsWith("PICodeValidation")){
        readData.remove("PICodeValidation").remove(":");
        this->m_xPICodeInfo.PICodeValidation = readData.toUShort();
    }else{
        qDebug() << " Can't find : " << QString("PICodeValidation");
        return false;
    }

    if(this->m_xPICodeInfo.PICodeCount > 11)
        return false;

    for(int i=0; i<this->m_xPICodeInfo.PICodeCount; i++)
    {
        readData = in.readLine();
        if(readData.startsWith("PICodeType")){
            readData.remove("PICodeType").remove(":");
            this->m_xPICodeInfo.PICodeType[i] = readData.toUShort();
        }else{
            qDebug() << " Can't find : " << QString("PICodeType");
            return false;
        }

        readData = in.readLine();
        if(readData.startsWith("PICodeValue")){
            readData.remove("PICodeValue").remove(":");
            this->m_xPICodeInfo.PICodeValue[i] = readData.toUShort();
        }else{
            qDebug() << " Can't find : " << QString("PICodeValue");
            return false;
        }
    }

    return true;
}

bool
DABChannelItem::saveData(QTextStream &out)
{
    /*
    names[FrequencyRole] = "frequency";
    //names[FrequencyLabelRole] = "frequencyLabel";
    names[EnsembleIDRole] = "ensembleID";
    names[EnsembleLabelRole] = "ensembleLabel";
    names[ServiceCountRole] = "serviceCount";

    names[ServiceIDRole] = "serviceID";
    names[ServiceTypeRole] = "serviceType";
    names[SubChIdRole] = "subChId";
    names[BitrateRole] = "bitrate";
    names[PSRole] = "ps";
    names[CharsetRole] = "charset";
    names[LabelRole] = "label";
    */
    QHash<int, QByteArray> names = this->roleNames();

    out << names.value(FrequencyRole)               <<":"<<this->m_iFrequency << endl;
    out << names.value(EnsembleIDRole)              <<":"<<this->m_iEnsembleID << endl;
    out << names.value(InternationalTableIDRole)    <<":"<<this->m_iInternationalTableID << endl;
    out << names.value(EnsembleLabelRole)           <<":"<<this->m_sEnsembleLabel << endl;
    out << names.value(ServiceCountRole)            <<":"<<this->m_iServiceCount << endl;
    out << names.value(ServiceIDRole)               <<":"<<this->m_iServiceID << endl;
    out << names.value(ServiceTypeRole)             <<":"<<this->m_iServiceType << endl;
    out << names.value(SubChIdRole)                 <<":"<<this->m_iSubChId << endl;
    out << names.value(BitrateRole)                 <<":"<<this->m_iBitrate << endl;
    out << names.value(PSRole)                      <<":"<<this->m_iPS << endl;
    out << names.value(CharsetRole)                 <<":"<<this->m_iCharset << endl;
    out << names.value(ProgrammeTypeRole)           <<":"<<this->m_iProgrameType << endl;
    out << names.value(LanguageRole)                <<":"<<this->m_iLanguage << endl;
    out << names.value(LabelRole)                   <<":"<<this->m_sLabel << endl;
    out << names.value(IsDABtoDABRole)              <<":"<<this->m_bIsDABtoDAB << endl;
    out << names.value(IsDABtoFMRole)               <<":"<<this->m_bIsDABtoFM << endl;
    out << names.value(IsNoSignalRole)              <<":"<<this->m_bIsNoSignal << endl;
    out << names.value(PresetIndexRole)             <<":"<<this->m_iPresetIndex << endl;
    out << names.value(LockRole)                    <<":"<<this->m_bLock << endl;
    out << "SubCHSize:"                             << this->m_iSubCHSize << endl;
    out << "Address:"                               << this->m_iAddress << endl;
    out << names.value(IsSkippedRole)               <<":"<<this->m_bIsSkipped << endl;

    out << "ASUFlags:" << this->m_xASUInfo.ASUFlags << endl;
    out << "ASUClusterCount:" << this->m_xASUInfo.ASUClusterCount << endl;
    for(int i=0; i<this->m_xASUInfo.ASUClusterCount; i++)
    {
        out <<"ASUClusterIDs:"<< this->m_xASUInfo.ASUClusterIDs[i] << endl;
    }

    out << "PICodeCount:" << this->m_xPICodeInfo.PICodeCount << endl;
    out << "PICodeValidation:" << this->m_xPICodeInfo.PICodeValidation << endl;
    for(int i=0; i<this->m_xPICodeInfo.PICodeCount; i++)
    {
        out << "PICodeType:" << this->m_xPICodeInfo.PICodeType[i] << endl;
        out << "PICodeValue:" << this->m_xPICodeInfo.PICodeValue[i] << endl;
    }

    return true;
}

void
DABChannelItem::setASUInfo(stASU_INFO* asuInfo)
{
    memset(&m_xASUInfo, 0x00, sizeof(m_xASUInfo));
    m_xASUInfo.ASUFlags = asuInfo->ASUFlags;
    m_xASUInfo.ASUClusterCount = asuInfo->ASUClusterCount;
    memcpy((quint8*)m_xASUInfo.ASUClusterIDs, (quint8*)asuInfo->ASUClusterIDs, sizeof(asuInfo->ASUClusterIDs));
}

void
DABChannelItem::setPICodeInfo(stPICODE_INFO* picodeInfo)
{
    memset(&m_xPICodeInfo, 0x00, sizeof(m_xPICodeInfo));
    m_xPICodeInfo.PICodeCount = picodeInfo->PICodeCount;
    m_xPICodeInfo.PICodeValidation = picodeInfo->PICodeValidation;
    memcpy((quint8*)m_xPICodeInfo.PICodeType, (quint8*)picodeInfo->PICodeType, sizeof(picodeInfo->PICodeType));
    memcpy((quint16*)m_xPICodeInfo.PICodeValue, (quint16*)picodeInfo->PICodeValue, sizeof(picodeInfo->PICodeValue));
}

void
DABChannelItem::setSupplementary(ASUInfo asuInfo, PICodeInfo picodeInfo)
{
    memset(&m_xASUInfo, 0x00, sizeof(m_xASUInfo));
    m_xASUInfo.ASUFlags = asuInfo.ASUFlags;
    m_xASUInfo.ASUClusterCount = asuInfo.ASUClusterCount;
    memcpy((quint8*)m_xASUInfo.ASUClusterIDs, (quint8*)asuInfo.ASUClusterIDs, sizeof(asuInfo.ASUClusterIDs));

    memset(&m_xPICodeInfo, 0x00, sizeof(m_xPICodeInfo));
    m_xPICodeInfo.PICodeCount = picodeInfo.PICodeCount;
    m_xPICodeInfo.PICodeValidation = picodeInfo.PICodeValidation;
    memcpy((quint8*)m_xPICodeInfo.PICodeType, (quint8*)picodeInfo.PICodeType, sizeof(picodeInfo.PICodeType));
    memcpy((quint16*)m_xPICodeInfo.PICodeValue, (quint16*)picodeInfo.PICodeValue, sizeof(picodeInfo.PICodeValue));
}

void
DABChannelItem::setLinkingInfo(DABEPGReservationItem* pItem)
{
    memset(&m_xASUInfo, 0x00, sizeof(m_xASUInfo));
    m_xASUInfo.ASUFlags = pItem->getAsuInfo().ASUFlags;
    m_xASUInfo.ASUClusterCount = pItem->getAsuInfo().ASUClusterCount;
    memcpy((quint8*)m_xASUInfo.ASUClusterIDs, (quint8*)pItem->getAsuInfo().ASUClusterIDs, sizeof(pItem->getAsuInfo().ASUClusterIDs));

    memset(&m_xPICodeInfo, 0x00, sizeof(m_xPICodeInfo));
    m_xPICodeInfo.PICodeCount = pItem->getPicodeInfo().PICodeCount;
    m_xPICodeInfo.PICodeValidation = pItem->getPicodeInfo().PICodeValidation;
    memcpy((quint8*)m_xPICodeInfo.PICodeType, (quint8*)pItem->getPicodeInfo().PICodeType, sizeof(pItem->getPicodeInfo().PICodeType));
    memcpy((quint16*)m_xPICodeInfo.PICodeValue, (quint16*)pItem->getPicodeInfo().PICodeValue, sizeof(pItem->getPicodeInfo().PICodeValue));
}

void
DABChannelItem::clearLiningInfo(void)
{
    memset(&m_xASUInfo, 0x00, sizeof(m_xASUInfo));
    memset(&m_xPICodeInfo, 0x00, sizeof(m_xPICodeInfo));
}

DABChannelItem &
DABChannelItem::operator =(DABChannelItem& a)
{
    //qDebug() << " DABChannelItem operator = called.(" << this->label() <<" = " << a.label()<<")";

    this->setFrequency(a.frequency());
    this->setEnsembleID(a.ensembleID());
    this->setInternationalTableID(a.internationalTableID());
    this->setEnsembleLabel(a.ensembleLabel());
    this->setServiceCount(a.serviceCount());

    this->setServiceID(a.serviceID());
    this->setServiceType(a.serviceType());
    this->setSubChId(a.subChId());
    this->setBitrate(a.bitrate());
    this->setPS(a.ps());
    this->setCharset(a.charset());
    this->setProgrameType(a.programeType());
    this->setLanguage(a.language());
    this->setLabel(a.label());
    this->setDABtoDAB(a.isDABtoDAB());
    this->setDABtoFM(a.isDABtoFM());
    this->setNoSignal(a.isNoSignal());
    this->setPresetIndex(a.presetIndex());
    this->setSubCHSize(a.subCHSize());
    this->setAddress(a.address());
    this->setSkip(a.isSkipped());

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
DABChannelItem::washOffData(void)
{
    m_iFrequency = 0;
    m_sFrequencyLabel = "7D";
    m_iEnsembleID = 0;
    m_iInternationalTableID = 0;
    m_sEnsembleLabel = "...";
    m_iServiceCount = 0;
    m_iServiceID = 0;
    m_iCER = 0;
    m_iServiceType = 0;
    m_iBitrate = 0;
    m_iCharset = 0;
    m_iProgrameType = 255;
    m_sProgrameTypeName = "";
    m_iLanguage = 0;
    m_sLabel = "...";
    m_bIsDABtoDAB = false;
    m_bIsDABtoFM = false;
    m_bIsNoSignal = false;
    m_iSubChId = 0;
    m_iPS = 0;
    m_iPresetIndex = -1;
    m_bLock = true;
    m_bIsSkipped = false;
    m_iSubCHSize = 0;
    m_iAddress = 0;
    memset(&m_xASUInfo, 0x00, sizeof(m_xASUInfo));
    memset(&m_xPICodeInfo, 0x00, sizeof(m_xPICodeInfo));
}
