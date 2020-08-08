/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_Video_SModel_PopToast.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_PopToast.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_Video_SModel_PopToast.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppMediaPlayer_Video_SModel_PopToast[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       1,   24, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      38,   37,   37,   37, 0x05,

 // slots: signature, parameters, type, tag, flags
      60,   37,   37,   37, 0x0a,

 // properties: name, type, flags
      86,   78, 0x0a495001,

 // properties: notify_signal_id
       0,

       0        // eod
};

static const char qt_meta_stringdata_AppMediaPlayer_Video_SModel_PopToast[] = {
    "AppMediaPlayer_Video_SModel_PopToast\0"
    "\0sTextChanged(QString)\0setSText(QString)\0"
    "QString\0sText\0"
};

void AppMediaPlayer_Video_SModel_PopToast::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppMediaPlayer_Video_SModel_PopToast *_t = static_cast<AppMediaPlayer_Video_SModel_PopToast *>(_o);
        switch (_id) {
        case 0: _t->sTextChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->setSText((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppMediaPlayer_Video_SModel_PopToast::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppMediaPlayer_Video_SModel_PopToast::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AppMediaPlayer_Video_SModel_PopToast,
      qt_meta_data_AppMediaPlayer_Video_SModel_PopToast, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppMediaPlayer_Video_SModel_PopToast::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppMediaPlayer_Video_SModel_PopToast::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppMediaPlayer_Video_SModel_PopToast::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppMediaPlayer_Video_SModel_PopToast))
        return static_cast<void*>(const_cast< AppMediaPlayer_Video_SModel_PopToast*>(this));
    return QObject::qt_metacast(_clname);
}

int AppMediaPlayer_Video_SModel_PopToast::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = sText(); break;
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
void AppMediaPlayer_Video_SModel_PopToast::sTextChanged(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
