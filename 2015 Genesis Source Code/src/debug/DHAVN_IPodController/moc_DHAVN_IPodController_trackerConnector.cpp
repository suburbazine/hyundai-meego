/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_IPodController_trackerConnector.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "TrackerConnector/include/DHAVN_IPodController_trackerConnector.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_IPodController_trackerConnector.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CTrackerConnector[] = {

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
      19,   18,   18,   18, 0x05,
      44,   34,   18,   18, 0x05,
      69,   62,   18,   18, 0x05,
     100,   94,   18,   18, 0x05,
     130,  128,   18,   18, 0x05,
     158,   18,   18,   18, 0x05,
     178,  173,   18,   18, 0x05,
     217,   18,   18,   18, 0x05,
     271,  242,   18,   18, 0x05,
     302,   18,   18,   18, 0x05,
     316,   18,   18,   18, 0x05,
     348,   18,   18,   18, 0x05,
     386,  380,   18,   18, 0x05,
     418,   18,   18,   18, 0x25,

 // slots: signature, parameters, type, tag, flags
     446,   18,   18,   18, 0x0a,
     493,  462,   18,   18, 0x0a,
     536,   18,   18,   18, 0x0a,
     593,  554,   18,   18, 0x0a,
     665,  636,   18,   18, 0x2a,
     703,   18,   18,   18, 0x0a,
     722,   18,   18,   18, 0x0a,
     740,  380,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CTrackerConnector[] = {
    "CTrackerConnector\0\0mainLoopQuit()\0"
    "nExitCode\0mainLoopQuit(int)\0status\0"
    "initDBSyncComplete(bool)\0listt\0"
    "artworkFetched(QStringList)\0,\0"
    "initAutoPlayReady(bool,int)\0dbThreadQuit()\0"
    ",,,,\0IPODConnected(QString,int,int,int,int)\0"
    "IpodTracksAfterSync(int)\0"
    "MasterTableCount,deviceCount\0"
    "IPodPlayableSongCount(int,int)\0"
    "requestQuit()\0OnCurrentTrackCountChanged(int)\0"
    "OnCurrentPlayingTrackCount(int)\0pause\0"
    "OnDBIndexingStatusChanged(bool)\0"
    "OnDBIndexingStatusChanged()\0startDBSyncEC()\0"
    "namesOfCategoryContents,length\0"
    "getListCompleteSlot(QMap<int,QString>,int)\0"
    "stopCurrentTask()\0"
    "albumNameList,artistNameList,forceFlag\0"
    "getArtworkEC(QStringList,QStringList,bool)\0"
    "albumNameList,artistNameList\0"
    "getArtworkEC(QStringList,QStringList)\0"
    "autoPlayComplete()\0requestQuitSlot()\0"
    "OnChangeDBIndexingStatus(bool)\0"
};

void CTrackerConnector::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CTrackerConnector *_t = static_cast<CTrackerConnector *>(_o);
        switch (_id) {
        case 0: _t->mainLoopQuit(); break;
        case 1: _t->mainLoopQuit((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->initDBSyncComplete((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->artworkFetched((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 4: _t->initAutoPlayReady((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->dbThreadQuit(); break;
        case 6: _t->IPODConnected((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 7: _t->IpodTracksAfterSync((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->IPodPlayableSongCount((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: _t->requestQuit(); break;
        case 10: _t->OnCurrentTrackCountChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->OnCurrentPlayingTrackCount((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->OnDBIndexingStatusChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->OnDBIndexingStatusChanged(); break;
        case 14: _t->startDBSyncEC(); break;
        case 15: _t->getListCompleteSlot((*reinterpret_cast< QMap<int,QString>(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 16: _t->stopCurrentTask(); break;
        case 17: _t->getArtworkEC((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 18: _t->getArtworkEC((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2]))); break;
        case 19: _t->autoPlayComplete(); break;
        case 20: _t->requestQuitSlot(); break;
        case 21: _t->OnChangeDBIndexingStatus((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CTrackerConnector::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CTrackerConnector::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CTrackerConnector,
      qt_meta_data_CTrackerConnector, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CTrackerConnector::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CTrackerConnector::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CTrackerConnector::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CTrackerConnector))
        return static_cast<void*>(const_cast< CTrackerConnector*>(this));
    return QObject::qt_metacast(_clname);
}

int CTrackerConnector::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void CTrackerConnector::mainLoopQuit()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CTrackerConnector::mainLoopQuit(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CTrackerConnector::initDBSyncComplete(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CTrackerConnector::artworkFetched(QStringList _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CTrackerConnector::initAutoPlayReady(bool _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CTrackerConnector::dbThreadQuit()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void CTrackerConnector::IPODConnected(QString _t1, int _t2, int _t3, int _t4, int _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CTrackerConnector::IpodTracksAfterSync(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void CTrackerConnector::IPodPlayableSongCount(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void CTrackerConnector::requestQuit()
{
    QMetaObject::activate(this, &staticMetaObject, 9, 0);
}

// SIGNAL 10
void CTrackerConnector::OnCurrentTrackCountChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void CTrackerConnector::OnCurrentPlayingTrackCount(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void CTrackerConnector::OnDBIndexingStatusChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}
QT_END_MOC_NAMESPACE
