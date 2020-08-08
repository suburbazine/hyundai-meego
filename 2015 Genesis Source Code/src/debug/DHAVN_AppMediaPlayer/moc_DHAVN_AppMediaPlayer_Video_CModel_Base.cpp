/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_Video_CModel_Base.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_Base.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_Video_CModel_Base.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppMediaPlayer_Video_CModel_Base[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: signature, parameters, type, tag, flags
      40,   34,   33,   33, 0x05,
     115,   34,   33,   33, 0x05,
     182,   34,   33,   33, 0x05,
     249,   34,   33,   33, 0x05,
     321,  312,   33,   33, 0x05,
     342,   34,   33,   33, 0x05,
     361,   34,   33,   33, 0x05,
     383,   33,   33,   33, 0x05,
     408,   33,   33,   33, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_AppMediaPlayer_Video_CModel_Base[] = {
    "AppMediaPlayer_Video_CModel_Base\0\0"
    "value\0"
    "playbackStatusChanged(AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATU"
    "S_T)\0"
    "randomModeChanged(AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_T)\0"
    "repeatModeChanged(AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_T)\0"
    "scanModeChanged(AppMediaPlayer_Video_CModel_Base::SCAN_MODE_T)\0"
    "position\0positionChanged(int)\0"
    "dirtyChanged(bool)\0draggingChanged(bool)\0"
    "loadingModeChanged(bool)\0"
    "removingModeChanged(bool)\0"
};

void AppMediaPlayer_Video_CModel_Base::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppMediaPlayer_Video_CModel_Base *_t = static_cast<AppMediaPlayer_Video_CModel_Base *>(_o);
        switch (_id) {
        case 0: _t->playbackStatusChanged((*reinterpret_cast< AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_T(*)>(_a[1]))); break;
        case 1: _t->randomModeChanged((*reinterpret_cast< AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_T(*)>(_a[1]))); break;
        case 2: _t->repeatModeChanged((*reinterpret_cast< AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_T(*)>(_a[1]))); break;
        case 3: _t->scanModeChanged((*reinterpret_cast< AppMediaPlayer_Video_CModel_Base::SCAN_MODE_T(*)>(_a[1]))); break;
        case 4: _t->positionChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->dirtyChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->draggingChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->loadingModeChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->removingModeChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppMediaPlayer_Video_CModel_Base::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppMediaPlayer_Video_CModel_Base::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AppMediaPlayer_Video_CModel_Base,
      qt_meta_data_AppMediaPlayer_Video_CModel_Base, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppMediaPlayer_Video_CModel_Base::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppMediaPlayer_Video_CModel_Base::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppMediaPlayer_Video_CModel_Base::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppMediaPlayer_Video_CModel_Base))
        return static_cast<void*>(const_cast< AppMediaPlayer_Video_CModel_Base*>(this));
    return QObject::qt_metacast(_clname);
}

int AppMediaPlayer_Video_CModel_Base::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void AppMediaPlayer_Video_CModel_Base::playbackStatusChanged(AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_T _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AppMediaPlayer_Video_CModel_Base::randomModeChanged(AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_T _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AppMediaPlayer_Video_CModel_Base::repeatModeChanged(AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_T _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void AppMediaPlayer_Video_CModel_Base::scanModeChanged(AppMediaPlayer_Video_CModel_Base::SCAN_MODE_T _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void AppMediaPlayer_Video_CModel_Base::positionChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void AppMediaPlayer_Video_CModel_Base::dirtyChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void AppMediaPlayer_Video_CModel_Base::draggingChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void AppMediaPlayer_Video_CModel_Base::loadingModeChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void AppMediaPlayer_Video_CModel_Base::removingModeChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_END_MOC_NAMESPACE
