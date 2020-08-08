/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_Video_ControllerVCD.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/controller/video/DHAVN_AppMediaPlayer_Video_ControllerVCD.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_Video_ControllerVCD.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppMediaPlayer_Video_ControllerVCD[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      42,   36,   35,   35, 0x05,
      69,   35,   35,   35, 0x05,

 // slots: signature, parameters, type, tag, flags
      83,   35,   35,   35, 0x08,
     108,  103,   35,   35, 0x08,
     145,  132,   35,   35, 0x08,
     178,  171,   35,   35, 0x08,
     207,  198,   35,   35, 0x08,
     237,  230,   35,   35, 0x08,
     256,   35,   35,   35, 0x08,
     318,  277,   35,   35, 0x08,
     363,  352,   35,   35, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_AppMediaPlayer_Video_ControllerVCD[] = {
    "AppMediaPlayer_Video_ControllerVCD\0\0"
    "bShow\0showPlaybackControls(bool)\0"
    "cancelFfRew()\0onTrackChanged(int)\0"
    "pTOC\0onTOCReceived(DvdcTOC*)\0nScreenState\0"
    "onScreenStateChanged(int)\0nState\0"
    "onStateChanged(int)\0position\0"
    "onPositionChanged(int)\0nError\0"
    "onHandleError(int)\0onRealSoundChanged()\0"
    "bPrev,nTotalIdx,nIdxInFolder,nFolderSize\0"
    "OnTrackBufferred(int,int,int,int)\0"
    "trackindex\0onUserTrackChanged(int)\0"
};

void AppMediaPlayer_Video_ControllerVCD::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppMediaPlayer_Video_ControllerVCD *_t = static_cast<AppMediaPlayer_Video_ControllerVCD *>(_o);
        switch (_id) {
        case 0: _t->showPlaybackControls((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->cancelFfRew(); break;
        case 2: _t->onTrackChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->onTOCReceived((*reinterpret_cast< DvdcTOC*(*)>(_a[1]))); break;
        case 4: _t->onScreenStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->onStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->onPositionChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->onHandleError((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->onRealSoundChanged(); break;
        case 9: _t->OnTrackBufferred((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 10: _t->onUserTrackChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppMediaPlayer_Video_ControllerVCD::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppMediaPlayer_Video_ControllerVCD::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AppMediaPlayer_Video_ControllerVCD,
      qt_meta_data_AppMediaPlayer_Video_ControllerVCD, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppMediaPlayer_Video_ControllerVCD::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppMediaPlayer_Video_ControllerVCD::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppMediaPlayer_Video_ControllerVCD::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppMediaPlayer_Video_ControllerVCD))
        return static_cast<void*>(const_cast< AppMediaPlayer_Video_ControllerVCD*>(this));
    if (!strcmp(_clname, "AppMediaPlayer_Video_ControllerBase"))
        return static_cast< AppMediaPlayer_Video_ControllerBase*>(const_cast< AppMediaPlayer_Video_ControllerVCD*>(this));
    return QObject::qt_metacast(_clname);
}

int AppMediaPlayer_Video_ControllerVCD::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void AppMediaPlayer_Video_ControllerVCD::showPlaybackControls(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AppMediaPlayer_Video_ControllerVCD::cancelFfRew()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
