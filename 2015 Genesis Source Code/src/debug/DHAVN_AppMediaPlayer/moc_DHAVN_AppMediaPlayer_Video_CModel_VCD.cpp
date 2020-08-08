/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_Video_CModel_VCD.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_VCD.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_Video_CModel_VCD.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppMediaPlayer_Video_CModel_VCD[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      39,   33,   32,   32, 0x05,
      62,   33,   32,   32, 0x05,
      87,   33,   32,   32, 0x05,
     119,   33,   32,   32, 0x05,
     146,   33,   32,   32, 0x05,
     174,   33,   32,   32, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_AppMediaPlayer_Video_CModel_VCD[] = {
    "AppMediaPlayer_Video_CModel_VCD\0\0value\0"
    "trackCountChanged(int)\0currentTrackChanged(int)\0"
    "isPbcMenuDisplayedChanged(bool)\0"
    "tuneWheelTrackChanged(int)\0"
    "isTrackLoadingChanged(bool)\0"
    "isPbcModeChanged(bool)\0"
};

void AppMediaPlayer_Video_CModel_VCD::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppMediaPlayer_Video_CModel_VCD *_t = static_cast<AppMediaPlayer_Video_CModel_VCD *>(_o);
        switch (_id) {
        case 0: _t->trackCountChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->currentTrackChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->isPbcMenuDisplayedChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->tuneWheelTrackChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->isTrackLoadingChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->isPbcModeChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppMediaPlayer_Video_CModel_VCD::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppMediaPlayer_Video_CModel_VCD::staticMetaObject = {
    { &AppMediaPlayer_Video_CModel_Base::staticMetaObject, qt_meta_stringdata_AppMediaPlayer_Video_CModel_VCD,
      qt_meta_data_AppMediaPlayer_Video_CModel_VCD, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppMediaPlayer_Video_CModel_VCD::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppMediaPlayer_Video_CModel_VCD::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppMediaPlayer_Video_CModel_VCD::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppMediaPlayer_Video_CModel_VCD))
        return static_cast<void*>(const_cast< AppMediaPlayer_Video_CModel_VCD*>(this));
    return AppMediaPlayer_Video_CModel_Base::qt_metacast(_clname);
}

int AppMediaPlayer_Video_CModel_VCD::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AppMediaPlayer_Video_CModel_Base::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void AppMediaPlayer_Video_CModel_VCD::trackCountChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AppMediaPlayer_Video_CModel_VCD::currentTrackChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AppMediaPlayer_Video_CModel_VCD::isPbcMenuDisplayedChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void AppMediaPlayer_Video_CModel_VCD::tuneWheelTrackChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void AppMediaPlayer_Video_CModel_VCD::isTrackLoadingChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void AppMediaPlayer_Video_CModel_VCD::isPbcModeChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE
