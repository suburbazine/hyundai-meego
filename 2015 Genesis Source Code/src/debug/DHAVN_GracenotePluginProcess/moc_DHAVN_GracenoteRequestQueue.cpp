/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_GracenoteRequestQueue.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_GracenoteRequestQueue.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_GracenoteRequestQueue.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CGracenoteRequestQueue[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      30,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      11,       // signalCount

 // signals: signature, parameters, type, tag, flags
      24,   23,   23,   23, 0x05,
      46,   39,   23,   23, 0x05,
      93,   76,   23,   23, 0x05,
     139,  125,   23,   23, 0x05,
     171,   23,   23,   23, 0x05,
     203,  185,   23,   23, 0x05,
     237,   23,   23,   23, 0x05,
     259,  255,   23,   23, 0x05,
     277,  255,   23,   23, 0x05,
     290,  255,   23,   23, 0x05,
     313,   39,   23,   23, 0x05,

 // slots: signature, parameters, type, tag, flags
     343,   23,   23,   23, 0x08,
     367,  359,   23,   23, 0x08,
     392,  384,   23,   23, 0x08,
     415,  384,   23,   23, 0x08,
     452,  439,   23,   23, 0x08,
     511,  484,   23,   23, 0x08,
     571,  544,   23,   23, 0x08,
     654,  607,   23,   23, 0x08,
     695,  484,   23,   23, 0x08,
     753,  736,   23,   23, 0x08,
     784,  736,   23,   23, 0x08,
     816,   23,   23,   23, 0x08,
     858,  837,   23,   23, 0x08,
     911,   23,   23,   23, 0x08,
     942,  929,   23,   23, 0x08,
     974,   23,   23,   23, 0x08,
     995,   23,   23,   23, 0x08,
    1009,   23,   23,   23, 0x08,
    1022,   23,   23,   23, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CGracenoteRequestQueue[] = {
    "CGracenoteRequestQueue\0\0DeQueueStart()\0"
    "status\0OnMusicIDCDDataReceived(bool)\0"
    "status,imagePath\0CoverArtRetrieved(bool,QString)\0"
    "imagePathList\0CoverArtsRetrieved(QStringList)\0"
    "ExitFromApp()\0status,deviceType\0"
    "GracenoteIndexingStatus(bool,int)\0"
    "GracenoteKilled()\0ver\0GNSDKVer(QString)\0"
    "GNDBVer(int)\0GNDBRegionVer(QString)\0"
    "GracenoteLaunchComplete(bool)\0"
    "KeepOnDequeue()\0variant\0onStart(QString)\0"
    "tocInfo\0onFetchCDData(QString)\0"
    "onDeleteCDData(QString)\0metaDataList\0"
    "onRetrieveCoverArt(QStringList)\0"
    "volume,serialNum,mountPath\0"
    "onEnableMLT(int,QString,QString)\0"
    "serialNum,mountPath,volume\0"
    "onUsbConnected(QString,QString,int)\0"
    "serialNum,startIndex,endIndex,syncState,volume\0"
    "onIPodConnected(QString,int,int,int,int)\0"
    "onHighLightedVolume(int,QString,QString)\0"
    "serialNum,volume\0onUsbDisconnected(QString,int)\0"
    "onIPodDisConnected(QString,int)\0"
    "onCloseCollections()\0albumList,artistList\0"
    "onRetrieveMultipleCoverArts(QStringList,QStringList)\0"
    "onTimerFinished()\0targetVolume\0"
    "onResyncAfterCopyOperation(int)\0"
    "onjukeBoxMLTSignal()\0onGetSDKVer()\0"
    "onGetDBVer()\0onGetDBRegionVer()\0"
};

void CGracenoteRequestQueue::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CGracenoteRequestQueue *_t = static_cast<CGracenoteRequestQueue *>(_o);
        switch (_id) {
        case 0: _t->DeQueueStart(); break;
        case 1: _t->OnMusicIDCDDataReceived((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->CoverArtRetrieved((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: _t->CoverArtsRetrieved((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 4: _t->ExitFromApp(); break;
        case 5: _t->GracenoteIndexingStatus((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->GracenoteKilled(); break;
        case 7: _t->GNSDKVer((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->GNDBVer((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->GNDBRegionVer((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->GracenoteLaunchComplete((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->KeepOnDequeue(); break;
        case 12: _t->onStart((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: _t->onFetchCDData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 14: _t->onDeleteCDData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 15: _t->onRetrieveCoverArt((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 16: _t->onEnableMLT((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 17: _t->onUsbConnected((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 18: _t->onIPodConnected((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 19: _t->onHighLightedVolume((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 20: _t->onUsbDisconnected((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 21: _t->onIPodDisConnected((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 22: _t->onCloseCollections(); break;
        case 23: _t->onRetrieveMultipleCoverArts((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2]))); break;
        case 24: _t->onTimerFinished(); break;
        case 25: _t->onResyncAfterCopyOperation((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 26: _t->onjukeBoxMLTSignal(); break;
        case 27: _t->onGetSDKVer(); break;
        case 28: _t->onGetDBVer(); break;
        case 29: _t->onGetDBRegionVer(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CGracenoteRequestQueue::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CGracenoteRequestQueue::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CGracenoteRequestQueue,
      qt_meta_data_CGracenoteRequestQueue, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CGracenoteRequestQueue::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CGracenoteRequestQueue::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CGracenoteRequestQueue::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CGracenoteRequestQueue))
        return static_cast<void*>(const_cast< CGracenoteRequestQueue*>(this));
    return QObject::qt_metacast(_clname);
}

int CGracenoteRequestQueue::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 30)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 30;
    }
    return _id;
}

// SIGNAL 0
void CGracenoteRequestQueue::DeQueueStart()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CGracenoteRequestQueue::OnMusicIDCDDataReceived(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CGracenoteRequestQueue::CoverArtRetrieved(bool _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CGracenoteRequestQueue::CoverArtsRetrieved(QStringList _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CGracenoteRequestQueue::ExitFromApp()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void CGracenoteRequestQueue::GracenoteIndexingStatus(bool _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CGracenoteRequestQueue::GracenoteKilled()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void CGracenoteRequestQueue::GNSDKVer(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void CGracenoteRequestQueue::GNDBVer(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void CGracenoteRequestQueue::GNDBRegionVer(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void CGracenoteRequestQueue::GracenoteLaunchComplete(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}
QT_END_MOC_NAMESPACE
