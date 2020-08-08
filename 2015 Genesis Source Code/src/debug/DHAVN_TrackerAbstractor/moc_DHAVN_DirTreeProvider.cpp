/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_DirTreeProvider.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_DirTreeProvider.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_DirTreeProvider.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CDirTreeThread[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   16,   15,   15, 0x05,
      97,   93,   15,   15, 0x05,
     203,  201,   15,   15, 0x05,
     266,   15,   15,   15, 0x05,
     277,   16,   15,   15, 0x05,
     330,  201,   15,   15, 0x05,
     416,  379,   15,   15, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_CDirTreeThread[] = {
    "CDirTreeThread\0\0,,\0"
    "DataDirectFetched(QVector<QStringList>,ETrackerAbstarctor_Query_Ids,ui"
    "nt)\0"
    ",,,\0"
    "DataDirectFetched(QVector<QStringList>,ETrackerAbstarctor_Query_Ids,ET"
    "rackerAbstarctor_VolumeType,uint)\0"
    ",\0DataFetched(QVector<QStringList>,ETrackerAbstarctor_Query_Ids)\0"
    "Error(int)\0"
    "ScanCompleted(ETrackerAbstarctor_VolumeType,int,int)\0"
    "ReadCompleted(ETrackerAbstarctor_VolumeType,int)\0"
    "volume,bRescanMode,audio,video,photo\0"
    "MediaFileFound(ETrackerAbstarctor_VolumeType,int,int,int,int)\0"
};

void CDirTreeThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CDirTreeThread *_t = static_cast<CDirTreeThread *>(_o);
        switch (_id) {
        case 0: _t->DataDirectFetched((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2])),(*reinterpret_cast< uint(*)>(_a[3]))); break;
        case 1: _t->DataDirectFetched((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2])),(*reinterpret_cast< ETrackerAbstarctor_VolumeType(*)>(_a[3])),(*reinterpret_cast< uint(*)>(_a[4]))); break;
        case 2: _t->DataFetched((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2]))); break;
        case 3: _t->Error((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->ScanCompleted((*reinterpret_cast< ETrackerAbstarctor_VolumeType(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 5: _t->ReadCompleted((*reinterpret_cast< ETrackerAbstarctor_VolumeType(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->MediaFileFound((*reinterpret_cast< ETrackerAbstarctor_VolumeType(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CDirTreeThread::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CDirTreeThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_CDirTreeThread,
      qt_meta_data_CDirTreeThread, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CDirTreeThread::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CDirTreeThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CDirTreeThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CDirTreeThread))
        return static_cast<void*>(const_cast< CDirTreeThread*>(this));
    return QThread::qt_metacast(_clname);
}

int CDirTreeThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void CDirTreeThread::DataDirectFetched(QVector<QStringList> _t1, ETrackerAbstarctor_Query_Ids _t2, uint _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CDirTreeThread::DataDirectFetched(QVector<QStringList> _t1, ETrackerAbstarctor_Query_Ids _t2, ETrackerAbstarctor_VolumeType _t3, uint _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CDirTreeThread::DataFetched(QVector<QStringList> _t1, ETrackerAbstarctor_Query_Ids _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CDirTreeThread::Error(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CDirTreeThread::ScanCompleted(ETrackerAbstarctor_VolumeType _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CDirTreeThread::ReadCompleted(ETrackerAbstarctor_VolumeType _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CDirTreeThread::MediaFileFound(ETrackerAbstarctor_VolumeType _t1, int _t2, int _t3, int _t4, int _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
static const uint qt_meta_data_CDirTreeProvider[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   18,   17,   17, 0x05,
      99,   95,   17,   17, 0x05,
     205,  203,   17,   17, 0x05,
     268,   17,   17,   17, 0x05,
     279,   18,   17,   17, 0x05,
     332,  203,   17,   17, 0x05,
     418,  381,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
     480,   18,   17,   17, 0x0a,
     535,  203,   17,   17, 0x0a,
     586,  381,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CDirTreeProvider[] = {
    "CDirTreeProvider\0\0,,\0"
    "DataDirectFetched(QVector<QStringList>,ETrackerAbstarctor_Query_Ids,ui"
    "nt)\0"
    ",,,\0"
    "DataDirectFetched(QVector<QStringList>,ETrackerAbstarctor_Query_Ids,ET"
    "rackerAbstarctor_VolumeType,uint)\0"
    ",\0DataFetched(QVector<QStringList>,ETrackerAbstarctor_Query_Ids)\0"
    "Error(int)\0"
    "ScanCompleted(ETrackerAbstarctor_VolumeType,int,int)\0"
    "ReadCompleted(ETrackerAbstarctor_VolumeType,int)\0"
    "volume,bRescanMode,audio,video,photo\0"
    "MediaFileFound(ETrackerAbstarctor_VolumeType,int,int,int,int)\0"
    "OnScanCompleted(ETrackerAbstarctor_VolumeType,int,int)\0"
    "OnReadCompleted(ETrackerAbstarctor_VolumeType,int)\0"
    "OnMediaFileFound(ETrackerAbstarctor_VolumeType,int,int,int,int)\0"
};

void CDirTreeProvider::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CDirTreeProvider *_t = static_cast<CDirTreeProvider *>(_o);
        switch (_id) {
        case 0: _t->DataDirectFetched((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2])),(*reinterpret_cast< uint(*)>(_a[3]))); break;
        case 1: _t->DataDirectFetched((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2])),(*reinterpret_cast< ETrackerAbstarctor_VolumeType(*)>(_a[3])),(*reinterpret_cast< uint(*)>(_a[4]))); break;
        case 2: _t->DataFetched((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2]))); break;
        case 3: _t->Error((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->ScanCompleted((*reinterpret_cast< ETrackerAbstarctor_VolumeType(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 5: _t->ReadCompleted((*reinterpret_cast< ETrackerAbstarctor_VolumeType(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->MediaFileFound((*reinterpret_cast< ETrackerAbstarctor_VolumeType(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 7: _t->OnScanCompleted((*reinterpret_cast< ETrackerAbstarctor_VolumeType(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 8: _t->OnReadCompleted((*reinterpret_cast< ETrackerAbstarctor_VolumeType(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: _t->OnMediaFileFound((*reinterpret_cast< ETrackerAbstarctor_VolumeType(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CDirTreeProvider::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CDirTreeProvider::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CDirTreeProvider,
      qt_meta_data_CDirTreeProvider, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CDirTreeProvider::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CDirTreeProvider::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CDirTreeProvider::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CDirTreeProvider))
        return static_cast<void*>(const_cast< CDirTreeProvider*>(this));
    return QObject::qt_metacast(_clname);
}

int CDirTreeProvider::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void CDirTreeProvider::DataDirectFetched(QVector<QStringList> _t1, ETrackerAbstarctor_Query_Ids _t2, uint _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CDirTreeProvider::DataDirectFetched(QVector<QStringList> _t1, ETrackerAbstarctor_Query_Ids _t2, ETrackerAbstarctor_VolumeType _t3, uint _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CDirTreeProvider::DataFetched(QVector<QStringList> _t1, ETrackerAbstarctor_Query_Ids _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CDirTreeProvider::Error(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CDirTreeProvider::ScanCompleted(ETrackerAbstarctor_VolumeType _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CDirTreeProvider::ReadCompleted(ETrackerAbstarctor_VolumeType _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CDirTreeProvider::MediaFileFound(ETrackerAbstarctor_VolumeType _t1, int _t2, int _t3, int _t4, int _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_END_MOC_NAMESPACE
