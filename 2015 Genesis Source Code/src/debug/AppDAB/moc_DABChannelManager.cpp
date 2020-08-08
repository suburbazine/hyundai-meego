/****************************************************************************
** Meta object code from reading C++ file 'DABChannelManager.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DABChannelManager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DABChannelManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DABChannelManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      27,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      12,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x05,
      44,   18,   18,   18, 0x05,
      65,   18,   18,   18, 0x05,
      76,   18,   18,   18, 0x05,
      96,   18,   18,   18, 0x05,
     137,  117,   18,   18, 0x05,
     159,   18,   18,   18, 0x05,
     195,  179,   18,   18, 0x05,
     268,  227,   18,   18, 0x05,
     355,  318,   18,   18, 0x05,
     491,  397,   18,   18, 0x05,
     664,  564,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
     874,  753,   18,   18, 0x0a,
    1040,  753,   18,   18, 0x0a,
    1205,  753,   18,   18, 0x0a,
    1375, 1370,   18,   18, 0x0a,
    1408,   18,   18,   18, 0x0a,
    1442,   18,   18,   18, 0x0a,
    1483,   18,   18,   18, 0x0a,

 // methods: signature, parameters, type, tag, flags
    1547, 1538, 1534,   18, 0x02,
    1587, 1568, 1534,   18, 0x02,
    1639, 1629, 1534,   18, 0x02,
    1669, 1629, 1534,   18, 0x02,
    1699, 1629, 1534,   18, 0x02,
    1731,   18,   18,   18, 0x02,
    1761, 1751, 1534,   18, 0x02,
    1799,   18, 1534,   18, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_DABChannelManager[] = {
    "DABChannelManager\0\0cmdReqSetSelectService()\0"
    "cmdWritePresetList()\0scanStop()\0"
    "cmdCheckTAService()\0cmdInitPICodeValue()\0"
    "bViewPostionChanged\0playIndexUpdate(bool)\0"
    "updateStationList()\0currPresetindex\0"
    "playIconUpdateInPresetList(int)\0"
    "serviceName,ensembleName,index,realIndex\0"
    "serviceNameForPresetList(QString,QString,int,int)\0"
    "ensembleName,frequency,sectionStatus\0"
    "sendEnsembleName(QString,quint32,QString)\0"
    "listIndex,frequency,ensembleName,serviceName,pty,ps,isDABtoDAB,isDABto"
    "FM,isNoSignal,bPlayIcon\0"
    "sendServiceName(int,quint32,QString,QString,int,int,bool,bool,bool,boo"
    "l)\0"
    "listIndex,frequency,ensembleName,serviceName,pty,ps,isDABtoDAB,isDABto"
    "FM,isNoSignal,bPlayIcon,index\0"
    "sendServiceNameForSortingBy(int,quint32,QString,QString,int,int,bool,b"
    "ool,bool,bool,int)\0"
    "freq,eId,iId,eLabel,sCount,sID,sType,subChId,bitrate,ps,charset,pty,la"
    "nguage,sLabel,asuInfo,picodeInfo,subCHSize,address\0"
    "onAddTempChannelItem(quint32,quint16,quint8,QString,quint8,quint32,qui"
    "nt8,quint8,quint16,quint8,quint8,quint8,quint8,QString,stASU_INFO,stPI"
    "CODE_INFO,quint8,quint16)\0"
    "onUpdateChannelItem(quint32,quint16,quint8,QString,quint8,quint32,quin"
    "t8,quint8,quint16,quint8,quint8,quint8,quint8,QString,stASU_INFO,stPIC"
    "ODE_INFO,quint8,quint16)\0"
    "onAddNewChannelItem(quint32,quint16,quint8,QString,quint8,quint32,quin"
    "t8,quint8,quint16,quint8,quint8,quint8,quint8,QString,stASU_INFO,stPIC"
    "ODE_INFO,quint8,quint16)\0"
    "freq\0onRemoveUnlockFrequency(quint32)\0"
    "onUpdateChannelListFromAutoScan()\0"
    "onUpdateChannelListFromReconfiguration()\0"
    "onUpdateChannelListFromReconfigurationNotChanged()\0"
    "int\0sortType\0loadStationList(int)\0"
    "frequency,isSelect\0"
    "loadCurrentFreguencyChannel(quint32,bool)\0"
    "frequency\0listSelectBySeekDown(quint32)\0"
    "loadListByLongSeekUp(quint32)\0"
    "loadListByLongSeekDown(quint32)\0"
    "deleteChannelList()\0sortingBy\0"
    "findPlayIndexFromStationList(QString)\0"
    "getPresetListItem()\0"
};

void DABChannelManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DABChannelManager *_t = static_cast<DABChannelManager *>(_o);
        switch (_id) {
        case 0: _t->cmdReqSetSelectService(); break;
        case 1: _t->cmdWritePresetList(); break;
        case 2: _t->scanStop(); break;
        case 3: _t->cmdCheckTAService(); break;
        case 4: _t->cmdInitPICodeValue(); break;
        case 5: _t->playIndexUpdate((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->updateStationList(); break;
        case 7: _t->playIconUpdateInPresetList((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->serviceNameForPresetList((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 9: _t->sendEnsembleName((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< quint32(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 10: _t->sendServiceName((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< quint32(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6])),(*reinterpret_cast< bool(*)>(_a[7])),(*reinterpret_cast< bool(*)>(_a[8])),(*reinterpret_cast< bool(*)>(_a[9])),(*reinterpret_cast< bool(*)>(_a[10]))); break;
        case 11: _t->sendServiceNameForSortingBy((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< quint32(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6])),(*reinterpret_cast< bool(*)>(_a[7])),(*reinterpret_cast< bool(*)>(_a[8])),(*reinterpret_cast< bool(*)>(_a[9])),(*reinterpret_cast< bool(*)>(_a[10])),(*reinterpret_cast< int(*)>(_a[11]))); break;
        case 12: _t->onAddTempChannelItem((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< quint16(*)>(_a[2])),(*reinterpret_cast< quint8(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< quint8(*)>(_a[5])),(*reinterpret_cast< quint32(*)>(_a[6])),(*reinterpret_cast< quint8(*)>(_a[7])),(*reinterpret_cast< quint8(*)>(_a[8])),(*reinterpret_cast< quint16(*)>(_a[9])),(*reinterpret_cast< quint8(*)>(_a[10])),(*reinterpret_cast< quint8(*)>(_a[11])),(*reinterpret_cast< quint8(*)>(_a[12])),(*reinterpret_cast< quint8(*)>(_a[13])),(*reinterpret_cast< QString(*)>(_a[14])),(*reinterpret_cast< stASU_INFO(*)>(_a[15])),(*reinterpret_cast< stPICODE_INFO(*)>(_a[16])),(*reinterpret_cast< quint8(*)>(_a[17])),(*reinterpret_cast< quint16(*)>(_a[18]))); break;
        case 13: _t->onUpdateChannelItem((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< quint16(*)>(_a[2])),(*reinterpret_cast< quint8(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< quint8(*)>(_a[5])),(*reinterpret_cast< quint32(*)>(_a[6])),(*reinterpret_cast< quint8(*)>(_a[7])),(*reinterpret_cast< quint8(*)>(_a[8])),(*reinterpret_cast< quint16(*)>(_a[9])),(*reinterpret_cast< quint8(*)>(_a[10])),(*reinterpret_cast< quint8(*)>(_a[11])),(*reinterpret_cast< quint8(*)>(_a[12])),(*reinterpret_cast< quint8(*)>(_a[13])),(*reinterpret_cast< QString(*)>(_a[14])),(*reinterpret_cast< stASU_INFO(*)>(_a[15])),(*reinterpret_cast< stPICODE_INFO(*)>(_a[16])),(*reinterpret_cast< quint8(*)>(_a[17])),(*reinterpret_cast< quint16(*)>(_a[18]))); break;
        case 14: _t->onAddNewChannelItem((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< quint16(*)>(_a[2])),(*reinterpret_cast< quint8(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< quint8(*)>(_a[5])),(*reinterpret_cast< quint32(*)>(_a[6])),(*reinterpret_cast< quint8(*)>(_a[7])),(*reinterpret_cast< quint8(*)>(_a[8])),(*reinterpret_cast< quint16(*)>(_a[9])),(*reinterpret_cast< quint8(*)>(_a[10])),(*reinterpret_cast< quint8(*)>(_a[11])),(*reinterpret_cast< quint8(*)>(_a[12])),(*reinterpret_cast< quint8(*)>(_a[13])),(*reinterpret_cast< QString(*)>(_a[14])),(*reinterpret_cast< stASU_INFO(*)>(_a[15])),(*reinterpret_cast< stPICODE_INFO(*)>(_a[16])),(*reinterpret_cast< quint8(*)>(_a[17])),(*reinterpret_cast< quint16(*)>(_a[18]))); break;
        case 15: _t->onRemoveUnlockFrequency((*reinterpret_cast< quint32(*)>(_a[1]))); break;
        case 16: _t->onUpdateChannelListFromAutoScan(); break;
        case 17: _t->onUpdateChannelListFromReconfiguration(); break;
        case 18: _t->onUpdateChannelListFromReconfigurationNotChanged(); break;
        case 19: { int _r = _t->loadStationList((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 20: { int _r = _t->loadCurrentFreguencyChannel((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 21: { int _r = _t->listSelectBySeekDown((*reinterpret_cast< quint32(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 22: { int _r = _t->loadListByLongSeekUp((*reinterpret_cast< quint32(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 23: { int _r = _t->loadListByLongSeekDown((*reinterpret_cast< quint32(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 24: _t->deleteChannelList(); break;
        case 25: { int _r = _t->findPlayIndexFromStationList((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 26: { int _r = _t->getPresetListItem();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DABChannelManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DABChannelManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DABChannelManager,
      qt_meta_data_DABChannelManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DABChannelManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DABChannelManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DABChannelManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DABChannelManager))
        return static_cast<void*>(const_cast< DABChannelManager*>(this));
    return QObject::qt_metacast(_clname);
}

int DABChannelManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 27)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 27;
    }
    return _id;
}

// SIGNAL 0
void DABChannelManager::cmdReqSetSelectService()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void DABChannelManager::cmdWritePresetList()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void DABChannelManager::scanStop()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void DABChannelManager::cmdCheckTAService()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void DABChannelManager::cmdInitPICodeValue()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void DABChannelManager::playIndexUpdate(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void DABChannelManager::updateStationList()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void DABChannelManager::playIconUpdateInPresetList(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void DABChannelManager::serviceNameForPresetList(QString _t1, QString _t2, int _t3, int _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void DABChannelManager::sendEnsembleName(QString _t1, quint32 _t2, QString _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void DABChannelManager::sendServiceName(int _t1, quint32 _t2, QString _t3, QString _t4, int _t5, int _t6, bool _t7, bool _t8, bool _t9, bool _t10)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)), const_cast<void*>(reinterpret_cast<const void*>(&_t7)), const_cast<void*>(reinterpret_cast<const void*>(&_t8)), const_cast<void*>(reinterpret_cast<const void*>(&_t9)), const_cast<void*>(reinterpret_cast<const void*>(&_t10)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void DABChannelManager::sendServiceNameForSortingBy(int _t1, quint32 _t2, QString _t3, QString _t4, int _t5, int _t6, bool _t7, bool _t8, bool _t9, bool _t10, int _t11)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)), const_cast<void*>(reinterpret_cast<const void*>(&_t7)), const_cast<void*>(reinterpret_cast<const void*>(&_t8)), const_cast<void*>(reinterpret_cast<const void*>(&_t9)), const_cast<void*>(reinterpret_cast<const void*>(&_t10)), const_cast<void*>(reinterpret_cast<const void*>(&_t11)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}
QT_END_MOC_NAMESPACE
