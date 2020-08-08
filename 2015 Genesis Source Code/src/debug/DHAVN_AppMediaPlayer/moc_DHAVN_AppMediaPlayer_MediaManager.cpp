/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_MediaManager.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/util/DHAVN_AppMediaPlayer_MediaManager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_MediaManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppMediaPlayer_MediaManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: signature, parameters, type, tag, flags
      52,   29,   28,   28, 0x05,
     115,  101,   28,   28, 0x05,
     158,   28,   28,   28, 0x05,
     186,  180,   28,   28, 0x05,
     234,  215,   28,   28, 0x05,
     291,   28,   28,   28, 0x05,
     308,   28,   28,   28, 0x05,
     322,   28,   28,   28, 0x05,

 // slots: signature, parameters, type, tag, flags
     385,  359,  354,   28, 0x0a,
     442,  431,   28,   28, 0x0a,
     509,  475,   28,   28, 0x0a,
     577,  431,   28,   28, 0x0a,
     613,  608,   28,   28, 0x0a,
     642,   28,   28,   28, 0x0a,
     662,  608,   28,   28, 0x0a,
     692,   28,   28,   28, 0x0a,
     713,   28,   28,   28, 0x08,
     738,   28,   28,   28, 0x08,
     755,   28,   28,   28, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_AppMediaPlayer_MediaManager[] = {
    "AppMediaPlayer_MediaManager\0\0"
    "aSource,aType,autoplay\0"
    "deviceAttached(MEDIA_SOURCE_T,MEDIA_TYPE_T,bool)\0"
    "aSource,aType\0deviceRemoved(MEDIA_SOURCE_T,MEDIA_TYPE_T)\0"
    "catreedeviceRemoved()\0state\0"
    "deckStateChanged(DISC_STATE)\0"
    "source,type,status\0"
    "sourceAvailableChanged(MEDIA_SOURCE_T,MEDIA_TYPE_T,bool)\0"
    "discLoaded(bool)\0discLoading()\0"
    "notifyIPodReady(MEDIA_SOURCE_T)\0bool\0"
    "portNumber,UUID,mountPath\0"
    "onUSBDevConnected(USB_PORT_T,QString,QString)\0"
    "portNumber\0onUSBDevDisconnected(USB_PORT_T)\0"
    "portNumber,UUID,mountPath,content\0"
    "onIPODConnected(USB_PORT_T,QString,QString,USB_DATA_CONTENT_MASK_T)\0"
    "onIPODDisconnected(USB_PORT_T)\0type\0"
    "onAUXConnected(MEDIA_TYPE_T)\0"
    "onAUXDisconnected()\0onAUX2Connected(MEDIA_TYPE_T)\0"
    "onAUX2Disconnected()\0onDeckScanCompleted(int)\0"
    "onDeckError(int)\0onGstPipelineState(int)\0"
};

void AppMediaPlayer_MediaManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppMediaPlayer_MediaManager *_t = static_cast<AppMediaPlayer_MediaManager *>(_o);
        switch (_id) {
        case 0: _t->deviceAttached((*reinterpret_cast< MEDIA_SOURCE_T(*)>(_a[1])),(*reinterpret_cast< MEDIA_TYPE_T(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 1: _t->deviceRemoved((*reinterpret_cast< MEDIA_SOURCE_T(*)>(_a[1])),(*reinterpret_cast< MEDIA_TYPE_T(*)>(_a[2]))); break;
        case 2: _t->catreedeviceRemoved(); break;
        case 3: _t->deckStateChanged((*reinterpret_cast< DISC_STATE(*)>(_a[1]))); break;
        case 4: _t->sourceAvailableChanged((*reinterpret_cast< MEDIA_SOURCE_T(*)>(_a[1])),(*reinterpret_cast< MEDIA_TYPE_T(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 5: _t->discLoaded((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->discLoading(); break;
        case 7: _t->notifyIPodReady((*reinterpret_cast< MEDIA_SOURCE_T(*)>(_a[1]))); break;
        case 8: { bool _r = _t->onUSBDevConnected((*reinterpret_cast< USB_PORT_T(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 9: _t->onUSBDevDisconnected((*reinterpret_cast< USB_PORT_T(*)>(_a[1]))); break;
        case 10: _t->onIPODConnected((*reinterpret_cast< USB_PORT_T(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< USB_DATA_CONTENT_MASK_T(*)>(_a[4]))); break;
        case 11: _t->onIPODDisconnected((*reinterpret_cast< USB_PORT_T(*)>(_a[1]))); break;
        case 12: _t->onAUXConnected((*reinterpret_cast< MEDIA_TYPE_T(*)>(_a[1]))); break;
        case 13: _t->onAUXDisconnected(); break;
        case 14: _t->onAUX2Connected((*reinterpret_cast< MEDIA_TYPE_T(*)>(_a[1]))); break;
        case 15: _t->onAUX2Disconnected(); break;
        case 16: _t->onDeckScanCompleted((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->onDeckError((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->onGstPipelineState((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppMediaPlayer_MediaManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppMediaPlayer_MediaManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AppMediaPlayer_MediaManager,
      qt_meta_data_AppMediaPlayer_MediaManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppMediaPlayer_MediaManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppMediaPlayer_MediaManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppMediaPlayer_MediaManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppMediaPlayer_MediaManager))
        return static_cast<void*>(const_cast< AppMediaPlayer_MediaManager*>(this));
    return QObject::qt_metacast(_clname);
}

int AppMediaPlayer_MediaManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    }
    return _id;
}

// SIGNAL 0
void AppMediaPlayer_MediaManager::deviceAttached(MEDIA_SOURCE_T _t1, MEDIA_TYPE_T _t2, bool _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AppMediaPlayer_MediaManager::deviceRemoved(MEDIA_SOURCE_T _t1, MEDIA_TYPE_T _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AppMediaPlayer_MediaManager::catreedeviceRemoved()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void AppMediaPlayer_MediaManager::deckStateChanged(DISC_STATE _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void AppMediaPlayer_MediaManager::sourceAvailableChanged(MEDIA_SOURCE_T _t1, MEDIA_TYPE_T _t2, bool _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void AppMediaPlayer_MediaManager::discLoaded(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void AppMediaPlayer_MediaManager::discLoading()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void AppMediaPlayer_MediaManager::notifyIPodReady(MEDIA_SOURCE_T _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_END_MOC_NAMESPACE
