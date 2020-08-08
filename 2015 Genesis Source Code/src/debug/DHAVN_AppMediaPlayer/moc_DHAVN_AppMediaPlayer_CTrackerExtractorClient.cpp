/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_CTrackerExtractorClient.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/util/audio/DHAVN_AppMediaPlayer_CTrackerExtractorClient.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_CTrackerExtractorClient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CTrackerExtractorClient[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       1,   64, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      47,   25,   24,   24, 0x05,
      86,   75,   24,   24, 0x05,
     117,  107,   24,   24, 0x05,
     140,   24,   24,   24, 0x05,
     158,   24,   24,   24, 0x05,
     177,   24,   24,   24, 0x05,

 // slots: signature, parameters, type, tag, flags
     232,  196,   24,   24, 0x08,
     293,  271,   24,   24, 0x28,
     357,  324,   24,   24, 0x08,
     400,   24,   24,   24, 0x08,

 // properties: name, type, flags
     422,  417, 0x01095001,

       0        // eod
};

static const char qt_meta_stringdata_CTrackerExtractorClient[] = {
    "CTrackerExtractorClient\0\0isReady,usbMiningPath\0"
    "minerFinished(bool,QString)\0percentage\0"
    "minerInProgress(int)\0mountPath\0"
    "needToUseQDir(QString)\0pauseMinerAsync()\0"
    "resumeMinerAsync()\0dbusTestAckAsync()\0"
    "minerType,minerStatus,usbMiningPath\0"
    "onMinerStatus(QString,QString,QString)\0"
    "minerType,minerStatus\0"
    "onMinerStatus(QString,QString)\0"
    "minerType,minerStatus,percentage\0"
    "onMinerStatusInDetail(QString,QString,int)\0"
    "onDBusSendTest()\0bool\0isMinerReady\0"
};

void CTrackerExtractorClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CTrackerExtractorClient *_t = static_cast<CTrackerExtractorClient *>(_o);
        switch (_id) {
        case 0: _t->minerFinished((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->minerInProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->needToUseQDir((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->pauseMinerAsync(); break;
        case 4: _t->resumeMinerAsync(); break;
        case 5: _t->dbusTestAckAsync(); break;
        case 6: _t->onMinerStatus((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 7: _t->onMinerStatus((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 8: _t->onMinerStatusInDetail((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 9: _t->onDBusSendTest(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CTrackerExtractorClient::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CTrackerExtractorClient::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CTrackerExtractorClient,
      qt_meta_data_CTrackerExtractorClient, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CTrackerExtractorClient::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CTrackerExtractorClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CTrackerExtractorClient::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CTrackerExtractorClient))
        return static_cast<void*>(const_cast< CTrackerExtractorClient*>(this));
    return QObject::qt_metacast(_clname);
}

int CTrackerExtractorClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = isMinerReady(); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::WriteProperty) {
        _id -= 1;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void CTrackerExtractorClient::minerFinished(bool _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CTrackerExtractorClient::minerInProgress(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CTrackerExtractorClient::needToUseQDir(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CTrackerExtractorClient::pauseMinerAsync()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void CTrackerExtractorClient::resumeMinerAsync()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void CTrackerExtractorClient::dbusTestAckAsync()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}
static const uint qt_meta_data_TrackerExtractorThread[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      24,   23,   23,   23, 0x0a,
      42,   23,   23,   23, 0x0a,
      61,   23,   23,   23, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_TrackerExtractorThread[] = {
    "TrackerExtractorThread\0\0pauseMinerAsync()\0"
    "resumeMinerAsync()\0dbusTestAckAsync()\0"
};

void TrackerExtractorThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        TrackerExtractorThread *_t = static_cast<TrackerExtractorThread *>(_o);
        switch (_id) {
        case 0: _t->pauseMinerAsync(); break;
        case 1: _t->resumeMinerAsync(); break;
        case 2: _t->dbusTestAckAsync(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData TrackerExtractorThread::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TrackerExtractorThread::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TrackerExtractorThread,
      qt_meta_data_TrackerExtractorThread, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TrackerExtractorThread::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TrackerExtractorThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TrackerExtractorThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TrackerExtractorThread))
        return static_cast<void*>(const_cast< TrackerExtractorThread*>(this));
    return QObject::qt_metacast(_clname);
}

int TrackerExtractorThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
