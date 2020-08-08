/****************************************************************************
** Meta object code from reading C++ file 'DABCommManager.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DABCommManager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DABCommManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DABCommManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      40,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      28,       // signalCount

 // signals: signature, parameters, type, tag, flags
     137,   16,   15,   15, 0x05,
     365,  298,   15,   15, 0x05,
     544,  428,   15,   15, 0x05,
     689,  623,   15,   15, 0x05,
     830,  776,   15,   15, 0x05,
     902,  897,   15,   15, 0x05,
     934,   15,   15,   15, 0x05,
     981,  954,   15,   15, 0x05,
    1033, 1024,   15,   15, 0x05,
    1058, 1052,   15,   15, 0x05,
    1104, 1096,   15,   15, 0x05,
    1151, 1143,   15,   15, 0x05,
    1182, 1178,   15,   15, 0x05,
    1325, 1252,   15,   15, 0x05,
    1444, 1418,   15,   15, 0x05,
    1548, 1514,   15,   15, 0x05,
    1623, 1585,   15,   15, 0x05,
    1666, 1659,   15,   15, 0x05,
    1687,   15,   15,   15, 0x05,
    1702,   15,   15,   15, 0x05,
    1724, 1717,   15,   15, 0x05,
    1756, 1750,   15,   15, 0x05,
    1789, 1717,   15,   15, 0x05,
    1837, 1815,   15,   15, 0x05,
    1876,   15,   15,   15, 0x05,
    1942, 1903,   15,   15, 0x05,
    1977,   15,   15,   15, 0x05,
    1999, 1994,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
    2054, 2026,   15,   15, 0x0a,
    2142, 2115,   15,   15, 0x0a,
    2257, 2207,   15,   15, 0x0a,
    2388, 2339,   15,   15, 0x0a,
    2474,   15,   15,   15, 0x0a,
    2515, 2495,   15,   15, 0x0a,
    2565, 2559,   15,   15, 0x0a,
    2604,   15,   15,   15, 0x0a,
    2642, 2637,   15,   15, 0x0a,
    2673, 2637,   15,   15, 0x0a,
    2706, 2637,   15,   15, 0x0a,
    2743, 2637,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DABCommManager[] = {
    "DABCommManager\0\0"
    "freq,eId,iId,eLabel,sCount,sID,sType,subChId,bitrate,ps,charset,pty,la"
    "nguage,sLabel,asuInfo,picodeInfo,subCHSize,address\0"
    "sendChannelItem(quint32,quint16,quint8,QString,quint8,quint32,quint8,q"
    "uint8,quint16,quint8,quint8,quint8,quint8,QString,stASU_INFO,stPICODE_"
    "INFO,quint8,quint16)\0"
    "mjd,serviceID,Hour,Minute,Second,Duration,ProgramLabel,Description\0"
    "cmdAddEPGItem(quint32,quint32,int,int,int,int,QString,QString)\0"
    "booloaderMajorVersion,bootloaderMinorVersion,kernelMajor,kernelMinor,a"
    "ppMajor,appMinor,appMicro,buildTime,bootCount\0"
    "rspGetVersion(quint8,quint8,quint8,quint8,quint8,quint8,quint8,QString"
    ",quint8)\0"
    "band,direction,FilterType,FilterValue,Frequency,ServiceID,SubChId\0"
    "rspSetSeekService(eREQ_SET_FULL_SCAN_BAND,quint8,quint8,quint8,quint32"
    ",quint32,quint8)\0"
    "frequency,serviceType,serviceID,subChId,bitrate,label\0"
    "rspSetSelectService(quint32,quint8,quint32,quint8,quint16,QString)\0"
    "flag\0rspSetAnnouncementFlag(quint16)\0"
    "rspGetServiceList()\0sDLS,sTitle,sAlbum,sArtist\0"
    "rspGetDLS(QString,QString,QString,QString)\0"
    "fileName\0rspGetSLS(QString)\0flags\0"
    "evtSendAnnouncementTAInvalid(quint16)\0"
    "sID,mjd\0removeDuplicatEPGData(quint32,quint32)\0"
    "mjd,sID\0rspGetEPG(quint32,quint32)\0"
    "m,s\0"
    "evtSendModuleStatus(eEVT_SEND_MODULE_STATUS,eEVT_SEND_SERVICE_STATUS)\0"
    "freq,serviceType,serviceID,SubChId,Bitrate,Pty,label,ensembleLabel,eve"
    "nt\0"
    "evtSendChangeService(int,int,int,int,int,int,QString,QString,eEVT_SEND"
    "_CHANGE_SERVICE_EVENT)\0"
    "label,flag,status,subChID\0"
    "evtSendAnnouncement(QString,int,eEVT_SEND_ANNOUNCEMENT_STATUS,quint8)\0"
    "year,month,day,hour,minute,second\0"
    "evtSendTime(int,int,int,int,int,int)\0"
    "CER,SNR,RSSI,CER_sub,SNR_sub,RSSI_sub\0"
    "evtSendQOS(int,int,int,int,int,int)\0"
    "update\0evtUpdateDLS(quint8)\0evtUpdateSLS()\0"
    "evtUpdateEPG()\0status\0evtSendSignalInfo(quint8)\0"
    "state\0evtServiceFollowingState(quint8)\0"
    "evtSendPinkStatus(quint8)\0"
    "status,frequency,lock\0"
    "evtAutoScanInfo(quint8,quint32,quint8)\0"
    "cmdAddStationLogo(QString)\0"
    "ProtectType,ProtectLevel,ProtectOption\0"
    "cmdAudioInfo(quint8,quint8,quint8)\0"
    "evtSendDtcUart()\0data\0cmdRspTPEGData(QByteArray)\0"
    "type,command,bArray,dataLen\0"
    "onProcessCommand(unsigned char,unsigned char,QByteArray,int)\0"
    "type,command,pData,dataLen\0"
    "onProcessCommand(unsigned char,unsigned char,unsigned char*,int)\0"
    "type,command,framepostion,bArray,dataLen,totalLen\0"
    "onSPIProcessCommand(unsigned char,unsigned char,unsigned char,QByteArr"
    "ay,int,int)\0"
    "type,command,framepostion,pData,dataLen,totalLen\0"
    "onSPIProcessCommand(unsigned char,unsigned char,unsigned char,unsigned"
    " char*,int,int)\0"
    "onUpdateOutputText()\0exitCode,exitStatus\0"
    "onProcessFinished(int,QProcess::ExitStatus)\0"
    "error\0onProcessError(QProcess::ProcessError)\0"
    "sendBackupCommandFromRebooting()\0time\0"
    "onReqSetAnnounceDelay(quint32)\0"
    "onReqSetAnnounceTimeout(quint32)\0"
    "onReqSetAnnounceWeakTimeout(quint32)\0"
    "onReqSetServLinkTimeout(quint32)\0"
};

void DABCommManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DABCommManager *_t = static_cast<DABCommManager *>(_o);
        switch (_id) {
        case 0: _t->sendChannelItem((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< quint16(*)>(_a[2])),(*reinterpret_cast< quint8(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< quint8(*)>(_a[5])),(*reinterpret_cast< quint32(*)>(_a[6])),(*reinterpret_cast< quint8(*)>(_a[7])),(*reinterpret_cast< quint8(*)>(_a[8])),(*reinterpret_cast< quint16(*)>(_a[9])),(*reinterpret_cast< quint8(*)>(_a[10])),(*reinterpret_cast< quint8(*)>(_a[11])),(*reinterpret_cast< quint8(*)>(_a[12])),(*reinterpret_cast< quint8(*)>(_a[13])),(*reinterpret_cast< QString(*)>(_a[14])),(*reinterpret_cast< stASU_INFO(*)>(_a[15])),(*reinterpret_cast< stPICODE_INFO(*)>(_a[16])),(*reinterpret_cast< quint8(*)>(_a[17])),(*reinterpret_cast< quint16(*)>(_a[18]))); break;
        case 1: _t->cmdAddEPGItem((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< quint32(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6])),(*reinterpret_cast< QString(*)>(_a[7])),(*reinterpret_cast< QString(*)>(_a[8]))); break;
        case 2: _t->rspGetVersion((*reinterpret_cast< quint8(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2])),(*reinterpret_cast< quint8(*)>(_a[3])),(*reinterpret_cast< quint8(*)>(_a[4])),(*reinterpret_cast< quint8(*)>(_a[5])),(*reinterpret_cast< quint8(*)>(_a[6])),(*reinterpret_cast< quint8(*)>(_a[7])),(*reinterpret_cast< QString(*)>(_a[8])),(*reinterpret_cast< quint8(*)>(_a[9]))); break;
        case 3: _t->rspSetSeekService((*reinterpret_cast< eREQ_SET_FULL_SCAN_BAND(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2])),(*reinterpret_cast< quint8(*)>(_a[3])),(*reinterpret_cast< quint8(*)>(_a[4])),(*reinterpret_cast< quint32(*)>(_a[5])),(*reinterpret_cast< quint32(*)>(_a[6])),(*reinterpret_cast< quint8(*)>(_a[7]))); break;
        case 4: _t->rspSetSelectService((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2])),(*reinterpret_cast< quint32(*)>(_a[3])),(*reinterpret_cast< quint8(*)>(_a[4])),(*reinterpret_cast< quint16(*)>(_a[5])),(*reinterpret_cast< QString(*)>(_a[6]))); break;
        case 5: _t->rspSetAnnouncementFlag((*reinterpret_cast< quint16(*)>(_a[1]))); break;
        case 6: _t->rspGetServiceList(); break;
        case 7: _t->rspGetDLS((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 8: _t->rspGetSLS((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->evtSendAnnouncementTAInvalid((*reinterpret_cast< quint16(*)>(_a[1]))); break;
        case 10: _t->removeDuplicatEPGData((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< quint32(*)>(_a[2]))); break;
        case 11: _t->rspGetEPG((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< quint32(*)>(_a[2]))); break;
        case 12: _t->evtSendModuleStatus((*reinterpret_cast< eEVT_SEND_MODULE_STATUS(*)>(_a[1])),(*reinterpret_cast< eEVT_SEND_SERVICE_STATUS(*)>(_a[2]))); break;
        case 13: _t->evtSendChangeService((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6])),(*reinterpret_cast< QString(*)>(_a[7])),(*reinterpret_cast< QString(*)>(_a[8])),(*reinterpret_cast< eEVT_SEND_CHANGE_SERVICE_EVENT(*)>(_a[9]))); break;
        case 14: _t->evtSendAnnouncement((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< eEVT_SEND_ANNOUNCEMENT_STATUS(*)>(_a[3])),(*reinterpret_cast< quint8(*)>(_a[4]))); break;
        case 15: _t->evtSendTime((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6]))); break;
        case 16: _t->evtSendQOS((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6]))); break;
        case 17: _t->evtUpdateDLS((*reinterpret_cast< quint8(*)>(_a[1]))); break;
        case 18: _t->evtUpdateSLS(); break;
        case 19: _t->evtUpdateEPG(); break;
        case 20: _t->evtSendSignalInfo((*reinterpret_cast< quint8(*)>(_a[1]))); break;
        case 21: _t->evtServiceFollowingState((*reinterpret_cast< quint8(*)>(_a[1]))); break;
        case 22: _t->evtSendPinkStatus((*reinterpret_cast< quint8(*)>(_a[1]))); break;
        case 23: _t->evtAutoScanInfo((*reinterpret_cast< quint8(*)>(_a[1])),(*reinterpret_cast< quint32(*)>(_a[2])),(*reinterpret_cast< quint8(*)>(_a[3]))); break;
        case 24: _t->cmdAddStationLogo((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 25: _t->cmdAudioInfo((*reinterpret_cast< quint8(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2])),(*reinterpret_cast< quint8(*)>(_a[3]))); break;
        case 26: _t->evtSendDtcUart(); break;
        case 27: _t->cmdRspTPEGData((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 28: _t->onProcessCommand((*reinterpret_cast< unsigned char(*)>(_a[1])),(*reinterpret_cast< unsigned char(*)>(_a[2])),(*reinterpret_cast< QByteArray(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 29: _t->onProcessCommand((*reinterpret_cast< unsigned char(*)>(_a[1])),(*reinterpret_cast< unsigned char(*)>(_a[2])),(*reinterpret_cast< unsigned char*(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 30: _t->onSPIProcessCommand((*reinterpret_cast< unsigned char(*)>(_a[1])),(*reinterpret_cast< unsigned char(*)>(_a[2])),(*reinterpret_cast< unsigned char(*)>(_a[3])),(*reinterpret_cast< QByteArray(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6]))); break;
        case 31: _t->onSPIProcessCommand((*reinterpret_cast< unsigned char(*)>(_a[1])),(*reinterpret_cast< unsigned char(*)>(_a[2])),(*reinterpret_cast< unsigned char(*)>(_a[3])),(*reinterpret_cast< unsigned char*(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6]))); break;
        case 32: _t->onUpdateOutputText(); break;
        case 33: _t->onProcessFinished((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QProcess::ExitStatus(*)>(_a[2]))); break;
        case 34: _t->onProcessError((*reinterpret_cast< QProcess::ProcessError(*)>(_a[1]))); break;
        case 35: _t->sendBackupCommandFromRebooting(); break;
        case 36: _t->onReqSetAnnounceDelay((*reinterpret_cast< quint32(*)>(_a[1]))); break;
        case 37: _t->onReqSetAnnounceTimeout((*reinterpret_cast< quint32(*)>(_a[1]))); break;
        case 38: _t->onReqSetAnnounceWeakTimeout((*reinterpret_cast< quint32(*)>(_a[1]))); break;
        case 39: _t->onReqSetServLinkTimeout((*reinterpret_cast< quint32(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DABCommManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DABCommManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DABCommManager,
      qt_meta_data_DABCommManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DABCommManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DABCommManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DABCommManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DABCommManager))
        return static_cast<void*>(const_cast< DABCommManager*>(this));
    return QObject::qt_metacast(_clname);
}

int DABCommManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 40)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 40;
    }
    return _id;
}

// SIGNAL 0
void DABCommManager::sendChannelItem(quint32 _t1, quint16 _t2, quint8 _t3, QString _t4, quint8 _t5, quint32 _t6, quint8 _t7, quint8 _t8, quint16 _t9, quint8 _t10, quint8 _t11, quint8 _t12, quint8 _t13, QString _t14, stASU_INFO _t15, stPICODE_INFO _t16, quint8 _t17, quint16 _t18)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)), const_cast<void*>(reinterpret_cast<const void*>(&_t7)), const_cast<void*>(reinterpret_cast<const void*>(&_t8)), const_cast<void*>(reinterpret_cast<const void*>(&_t9)), const_cast<void*>(reinterpret_cast<const void*>(&_t10)), const_cast<void*>(reinterpret_cast<const void*>(&_t11)), const_cast<void*>(reinterpret_cast<const void*>(&_t12)), const_cast<void*>(reinterpret_cast<const void*>(&_t13)), const_cast<void*>(reinterpret_cast<const void*>(&_t14)), const_cast<void*>(reinterpret_cast<const void*>(&_t15)), const_cast<void*>(reinterpret_cast<const void*>(&_t16)), const_cast<void*>(reinterpret_cast<const void*>(&_t17)), const_cast<void*>(reinterpret_cast<const void*>(&_t18)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DABCommManager::cmdAddEPGItem(quint32 _t1, quint32 _t2, int _t3, int _t4, int _t5, int _t6, QString _t7, QString _t8)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)), const_cast<void*>(reinterpret_cast<const void*>(&_t7)), const_cast<void*>(reinterpret_cast<const void*>(&_t8)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DABCommManager::rspGetVersion(quint8 _t1, quint8 _t2, quint8 _t3, quint8 _t4, quint8 _t5, quint8 _t6, quint8 _t7, QString _t8, quint8 _t9)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)), const_cast<void*>(reinterpret_cast<const void*>(&_t7)), const_cast<void*>(reinterpret_cast<const void*>(&_t8)), const_cast<void*>(reinterpret_cast<const void*>(&_t9)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DABCommManager::rspSetSeekService(eREQ_SET_FULL_SCAN_BAND _t1, quint8 _t2, quint8 _t3, quint8 _t4, quint32 _t5, quint32 _t6, quint8 _t7)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)), const_cast<void*>(reinterpret_cast<const void*>(&_t7)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void DABCommManager::rspSetSelectService(quint32 _t1, quint8 _t2, quint32 _t3, quint8 _t4, quint16 _t5, QString _t6)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void DABCommManager::rspSetAnnouncementFlag(quint16 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void DABCommManager::rspGetServiceList()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void DABCommManager::rspGetDLS(QString _t1, QString _t2, QString _t3, QString _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void DABCommManager::rspGetSLS(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void DABCommManager::evtSendAnnouncementTAInvalid(quint16 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void DABCommManager::removeDuplicatEPGData(quint32 _t1, quint32 _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void DABCommManager::rspGetEPG(quint32 _t1, quint32 _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void DABCommManager::evtSendModuleStatus(eEVT_SEND_MODULE_STATUS _t1, eEVT_SEND_SERVICE_STATUS _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void DABCommManager::evtSendChangeService(int _t1, int _t2, int _t3, int _t4, int _t5, int _t6, QString _t7, QString _t8, eEVT_SEND_CHANGE_SERVICE_EVENT _t9)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)), const_cast<void*>(reinterpret_cast<const void*>(&_t7)), const_cast<void*>(reinterpret_cast<const void*>(&_t8)), const_cast<void*>(reinterpret_cast<const void*>(&_t9)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void DABCommManager::evtSendAnnouncement(QString _t1, int _t2, eEVT_SEND_ANNOUNCEMENT_STATUS _t3, quint8 _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void DABCommManager::evtSendTime(int _t1, int _t2, int _t3, int _t4, int _t5, int _t6)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void DABCommManager::evtSendQOS(int _t1, int _t2, int _t3, int _t4, int _t5, int _t6)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void DABCommManager::evtUpdateDLS(quint8 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}

// SIGNAL 18
void DABCommManager::evtUpdateSLS()
{
    QMetaObject::activate(this, &staticMetaObject, 18, 0);
}

// SIGNAL 19
void DABCommManager::evtUpdateEPG()
{
    QMetaObject::activate(this, &staticMetaObject, 19, 0);
}

// SIGNAL 20
void DABCommManager::evtSendSignalInfo(quint8 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}

// SIGNAL 21
void DABCommManager::evtServiceFollowingState(quint8 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 21, _a);
}

// SIGNAL 22
void DABCommManager::evtSendPinkStatus(quint8 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 22, _a);
}

// SIGNAL 23
void DABCommManager::evtAutoScanInfo(quint8 _t1, quint32 _t2, quint8 _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 23, _a);
}

// SIGNAL 24
void DABCommManager::cmdAddStationLogo(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 24, _a);
}

// SIGNAL 25
void DABCommManager::cmdAudioInfo(quint8 _t1, quint8 _t2, quint8 _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 25, _a);
}

// SIGNAL 26
void DABCommManager::evtSendDtcUart()
{
    QMetaObject::activate(this, &staticMetaObject, 26, 0);
}

// SIGNAL 27
void DABCommManager::cmdRspTPEGData(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 27, _a);
}
QT_END_MOC_NAMESPACE
