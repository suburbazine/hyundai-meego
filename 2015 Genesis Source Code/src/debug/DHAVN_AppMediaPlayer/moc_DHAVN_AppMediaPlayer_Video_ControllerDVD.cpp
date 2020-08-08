/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_Video_ControllerDVD.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/controller/video/DHAVN_AppMediaPlayer_Video_ControllerDVD.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_Video_ControllerDVD.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppMediaPlayer_Video_ControllerDVD[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      49,   36,   35,   35, 0x05,
      73,   35,   35,   35, 0x05,

 // slots: signature, parameters, type, tag, flags
      87,   35,   35,   35, 0x08,
     140,  107,   35,   35, 0x08,
     182,  171,   35,   35, 0x08,
     226,  212,   35,   35, 0x08,
     261,  254,   35,   35, 0x08,
     287,   35,   35,   35, 0x08,
     307,   36,   35,   35, 0x08,
     340,  333,   35,   35, 0x08,
     367,  360,   35,   35, 0x08,
     386,   35,   35,   35, 0x08,
     448,  407,   35,   35, 0x08,
     493,  482,   35,   35, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_AppMediaPlayer_Video_ControllerDVD[] = {
    "AppMediaPlayer_Video_ControllerDVD\0\0"
    "nScreenState\0screenStateChanged(int)\0"
    "cancelFfRew()\0onTrackChanged(int)\0"
    "nTitleNo,nChapterNo,nChapterSize\0"
    "onSearchCompleted(int,int,int)\0"
    "nAudioCode\0onActiveAudioLangChanged(int)\0"
    "nSubtitleCode\0onActiveCaptionChanged(int)\0"
    "nAngle\0onActiveAngleChanged(int)\0"
    "OnDeckLangChanged()\0onScreenStateChanged(int)\0"
    "nState\0onStateChanged(int)\0nError\0"
    "onHandleError(int)\0onRealSoundChanged()\0"
    "bPrev,nTotalIdx,nIdxInFolder,nFolderSize\0"
    "OnTrackBufferred(int,int,int,int)\0"
    "trackindex\0onUserTrackChanged(int)\0"
};

void AppMediaPlayer_Video_ControllerDVD::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppMediaPlayer_Video_ControllerDVD *_t = static_cast<AppMediaPlayer_Video_ControllerDVD *>(_o);
        switch (_id) {
        case 0: _t->screenStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->cancelFfRew(); break;
        case 2: _t->onTrackChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->onSearchCompleted((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 4: _t->onActiveAudioLangChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->onActiveCaptionChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->onActiveAngleChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->OnDeckLangChanged(); break;
        case 8: _t->onScreenStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->onStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->onHandleError((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->onRealSoundChanged(); break;
        case 12: _t->OnTrackBufferred((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 13: _t->onUserTrackChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppMediaPlayer_Video_ControllerDVD::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppMediaPlayer_Video_ControllerDVD::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AppMediaPlayer_Video_ControllerDVD,
      qt_meta_data_AppMediaPlayer_Video_ControllerDVD, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppMediaPlayer_Video_ControllerDVD::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppMediaPlayer_Video_ControllerDVD::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppMediaPlayer_Video_ControllerDVD::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppMediaPlayer_Video_ControllerDVD))
        return static_cast<void*>(const_cast< AppMediaPlayer_Video_ControllerDVD*>(this));
    if (!strcmp(_clname, "AppMediaPlayer_Video_ControllerBase"))
        return static_cast< AppMediaPlayer_Video_ControllerBase*>(const_cast< AppMediaPlayer_Video_ControllerDVD*>(this));
    return QObject::qt_metacast(_clname);
}

int AppMediaPlayer_Video_ControllerDVD::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void AppMediaPlayer_Video_ControllerDVD::screenStateChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AppMediaPlayer_Video_ControllerDVD::cancelFfRew()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
