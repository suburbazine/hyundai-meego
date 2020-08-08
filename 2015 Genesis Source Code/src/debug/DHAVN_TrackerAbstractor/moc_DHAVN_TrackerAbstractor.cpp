/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_TrackerAbstractor.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_TrackerAbstractor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_TrackerAbstractor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CTrackerAbstractor[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      22,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      14,       // signalCount

 // signals: signature, parameters, type, tag, flags
      44,   20,   19,   19, 0x05,
     134,  117,   19,   19, 0x25,
     237,  202,   19,   19, 0x05,
     368,  340,   19,   19, 0x25,
     497,  466,   19,   19, 0x05,
     598,  574,   19,   19, 0x25,
     678,  670,   19,   19, 0x05,
     694,   19,   19,   19, 0x25,
     705,  670,   19,   19, 0x05,
     728,   19,   19,   19, 0x25,
     776,  746,   19,   19, 0x05,
     853,  821,   19,   19, 0x05,
     926,  906,   19,   19, 0x05,
    1016,  975,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
    1078,  117,   19,   19, 0x08,
    1142,  574,   19,   19, 0x08,
    1221, 1215,   19,   19, 0x08,
    1240,   19,   19,   19, 0x08,
    1257,   20,   19,   19, 0x08,
    1333,  202,   19,   19, 0x08,
    1449, 1439,   19,   19, 0x0a,
    1500, 1482,   19,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CTrackerAbstractor[] = {
    "CTrackerAbstractor\0\0dataList,queryId,reqUID\0"
    "RequesteComplete(QVector<QStringList>,ETrackerAbstarctor_Query_Ids,uin"
    "t)\0"
    "dataList,queryId\0"
    "RequesteComplete(QVector<QStringList>,ETrackerAbstarctor_Query_Ids)\0"
    "dataList,queryId,volumeType,reqUID\0"
    "RequesteComplete(QVector<QStringList>,ETrackerAbstarctor_Query_Ids,ETr"
    "ackerAbstarctor_VolumeType,uint)\0"
    "dataList,queryId,volumeType\0"
    "RequesteComplete(QVector<QStringList>,ETrackerAbstarctor_Query_Ids,ETr"
    "ackerAbstarctor_VolumeType)\0"
    "dataList,queryId,offset,reqUID\0"
    "RequesteComplete(QVector<QStringList>,ETrackerAbstarctor_Query_Ids,int"
    ",uint)\0"
    "dataList,queryId,offset\0"
    "RequesteComplete(QVector<QStringList>,ETrackerAbstarctor_Query_Ids,int"
    ")\0"
    ",reqUID\0Error(int,uint)\0Error(int)\0"
    "RequestState(int,uint)\0RequestState(int)\0"
    "albumList,artistList,deviceid\0"
    "getCoverArt(QStringList,QStringList,QString)\0"
    "volumeType,bRescanMode,bSuccess\0"
    "ScanCompleted(ETrackerAbstarctor_VolumeType,int,int)\0"
    "volumeType,bSuccess\0"
    "ReadCompleted(ETrackerAbstarctor_VolumeType,int)\0"
    "volumeType,bRescanMode,audio,video,photo\0"
    "MediaFileFound(ETrackerAbstarctor_VolumeType,int,int,int,int)\0"
    "DataReceived(QVector<QStringList>,ETrackerAbstarctor_Query_Ids)\0"
    "DataLimitReceived(QVector<QStringList>,ETrackerAbstarctor_Query_Ids,in"
    "t)\0"
    "error\0ErrorReceived(int)\0RequestTimeOut()\0"
    "DirTreeDataReceived(QVector<QStringList>,ETrackerAbstarctor_Query_Ids,"
    "uint)\0"
    "DirTreeDataReceived(QVector<QStringList>,ETrackerAbstarctor_Query_Ids,"
    "ETrackerAbstarctor_VolumeType,uint)\0"
    "coverArts\0onCoverArtsFromIPOD(QStringList)\0"
    "albumList,urlList\0"
    "getCoverArtSlot(QStringList,QStringList)\0"
};

void CTrackerAbstractor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CTrackerAbstractor *_t = static_cast<CTrackerAbstractor *>(_o);
        switch (_id) {
        case 0: _t->RequesteComplete((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2])),(*reinterpret_cast< uint(*)>(_a[3]))); break;
        case 1: _t->RequesteComplete((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2]))); break;
        case 2: _t->RequesteComplete((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2])),(*reinterpret_cast< ETrackerAbstarctor_VolumeType(*)>(_a[3])),(*reinterpret_cast< uint(*)>(_a[4]))); break;
        case 3: _t->RequesteComplete((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2])),(*reinterpret_cast< ETrackerAbstarctor_VolumeType(*)>(_a[3]))); break;
        case 4: _t->RequesteComplete((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< uint(*)>(_a[4]))); break;
        case 5: _t->RequesteComplete((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 6: _t->Error((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2]))); break;
        case 7: _t->Error((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->RequestState((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2]))); break;
        case 9: _t->RequestState((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->getCoverArt((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 11: _t->ScanCompleted((*reinterpret_cast< ETrackerAbstarctor_VolumeType(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 12: _t->ReadCompleted((*reinterpret_cast< ETrackerAbstarctor_VolumeType(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 13: _t->MediaFileFound((*reinterpret_cast< ETrackerAbstarctor_VolumeType(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 14: _t->DataReceived((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2]))); break;
        case 15: _t->DataLimitReceived((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 16: _t->ErrorReceived((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->RequestTimeOut(); break;
        case 18: _t->DirTreeDataReceived((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2])),(*reinterpret_cast< uint(*)>(_a[3]))); break;
        case 19: _t->DirTreeDataReceived((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2])),(*reinterpret_cast< ETrackerAbstarctor_VolumeType(*)>(_a[3])),(*reinterpret_cast< uint(*)>(_a[4]))); break;
        case 20: _t->onCoverArtsFromIPOD((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 21: _t->getCoverArtSlot((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CTrackerAbstractor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CTrackerAbstractor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CTrackerAbstractor,
      qt_meta_data_CTrackerAbstractor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CTrackerAbstractor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CTrackerAbstractor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CTrackerAbstractor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CTrackerAbstractor))
        return static_cast<void*>(const_cast< CTrackerAbstractor*>(this));
    return QObject::qt_metacast(_clname);
}

int CTrackerAbstractor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    }
    return _id;
}

// SIGNAL 0
void CTrackerAbstractor::RequesteComplete(QVector<QStringList> _t1, ETrackerAbstarctor_Query_Ids _t2, uint _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 2
void CTrackerAbstractor::RequesteComplete(QVector<QStringList> _t1, ETrackerAbstarctor_Query_Ids _t2, ETrackerAbstarctor_VolumeType _t3, uint _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 4
void CTrackerAbstractor::RequesteComplete(QVector<QStringList> _t1, ETrackerAbstarctor_Query_Ids _t2, int _t3, uint _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 6
void CTrackerAbstractor::Error(int _t1, uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 8
void CTrackerAbstractor::RequestState(int _t1, uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 10
void CTrackerAbstractor::getCoverArt(QStringList _t1, QStringList _t2, QString _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void CTrackerAbstractor::ScanCompleted(ETrackerAbstarctor_VolumeType _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void CTrackerAbstractor::ReadCompleted(ETrackerAbstarctor_VolumeType _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void CTrackerAbstractor::MediaFileFound(ETrackerAbstarctor_VolumeType _t1, int _t2, int _t3, int _t4, int _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}
QT_END_MOC_NAMESPACE
