/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_Video_SModel_Divx.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_Divx.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_Video_SModel_Divx.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppMediaPlayer_Video_SModel_Divx[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       3,   44, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      34,   33,   33,   33, 0x05,
      58,   33,   33,   33, 0x05,
      84,   33,   33,   33, 0x05,

 // slots: signature, parameters, type, tag, flags
     109,   33,   33,   33, 0x0a,
     129,   33,   33,   33, 0x0a,
     151,   33,   33,   33, 0x0a,

 // properties: name, type, flags
     180,  172, 0x0a495001,
     188,  172, 0x0a495001,
     198,  172, 0x0a495001,

 // properties: notify_signal_id
       0,
       1,
       2,

       0        // eod
};

static const char qt_meta_stringdata_AppMediaPlayer_Video_SModel_Divx[] = {
    "AppMediaPlayer_Video_SModel_Divx\0\0"
    "regCodeChanged(QString)\0"
    "deregCodeChanged(QString)\0"
    "scrStateChanged(QString)\0setRegCode(QString)\0"
    "setDeregCode(QString)\0setScrState(QString)\0"
    "QString\0regCode\0deregCode\0scrState\0"
};

void AppMediaPlayer_Video_SModel_Divx::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppMediaPlayer_Video_SModel_Divx *_t = static_cast<AppMediaPlayer_Video_SModel_Divx *>(_o);
        switch (_id) {
        case 0: _t->regCodeChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->deregCodeChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->scrStateChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->setRegCode((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->setDeregCode((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->setScrState((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppMediaPlayer_Video_SModel_Divx::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppMediaPlayer_Video_SModel_Divx::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AppMediaPlayer_Video_SModel_Divx,
      qt_meta_data_AppMediaPlayer_Video_SModel_Divx, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppMediaPlayer_Video_SModel_Divx::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppMediaPlayer_Video_SModel_Divx::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppMediaPlayer_Video_SModel_Divx::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppMediaPlayer_Video_SModel_Divx))
        return static_cast<void*>(const_cast< AppMediaPlayer_Video_SModel_Divx*>(this));
    return QObject::qt_metacast(_clname);
}

int AppMediaPlayer_Video_SModel_Divx::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = regCode(); break;
        case 1: *reinterpret_cast< QString*>(_v) = deregCode(); break;
        case 2: *reinterpret_cast< QString*>(_v) = scrState(); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::WriteProperty) {
        _id -= 3;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void AppMediaPlayer_Video_SModel_Divx::regCodeChanged(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AppMediaPlayer_Video_SModel_Divx::deregCodeChanged(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AppMediaPlayer_Video_SModel_Divx::scrStateChanged(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
