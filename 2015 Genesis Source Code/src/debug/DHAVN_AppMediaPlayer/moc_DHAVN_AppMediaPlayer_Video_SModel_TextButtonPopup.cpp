/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_Video_SModel_TextButtonPopup.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_TextButtonPopup.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_Video_SModel_TextButtonPopup.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppMediaPlayer_Video_SModel_TextButtonPopup[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       2,   34, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      45,   44,   44,   44, 0x05,
      67,   44,   44,   44, 0x05,

 // slots: signature, parameters, type, tag, flags
      94,   44,   44,   44, 0x0a,
     112,   44,   44,   44, 0x0a,

 // properties: name, type, flags
     143,  135, 0x0a495001,
     154,  149, 0x01495001,

 // properties: notify_signal_id
       0,
       1,

       0        // eod
};

static const char qt_meta_stringdata_AppMediaPlayer_Video_SModel_TextButtonPopup[] = {
    "AppMediaPlayer_Video_SModel_TextButtonPopup\0"
    "\0sTextChanged(QString)\0"
    "isWarningIconChanged(bool)\0setSText(QString)\0"
    "setIsWarningIcon(bool)\0QString\0sText\0"
    "bool\0isWarningIcon\0"
};

void AppMediaPlayer_Video_SModel_TextButtonPopup::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppMediaPlayer_Video_SModel_TextButtonPopup *_t = static_cast<AppMediaPlayer_Video_SModel_TextButtonPopup *>(_o);
        switch (_id) {
        case 0: _t->sTextChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->isWarningIconChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->setSText((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->setIsWarningIcon((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppMediaPlayer_Video_SModel_TextButtonPopup::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppMediaPlayer_Video_SModel_TextButtonPopup::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AppMediaPlayer_Video_SModel_TextButtonPopup,
      qt_meta_data_AppMediaPlayer_Video_SModel_TextButtonPopup, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppMediaPlayer_Video_SModel_TextButtonPopup::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppMediaPlayer_Video_SModel_TextButtonPopup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppMediaPlayer_Video_SModel_TextButtonPopup::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppMediaPlayer_Video_SModel_TextButtonPopup))
        return static_cast<void*>(const_cast< AppMediaPlayer_Video_SModel_TextButtonPopup*>(this));
    return QObject::qt_metacast(_clname);
}

int AppMediaPlayer_Video_SModel_TextButtonPopup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = sText(); break;
        case 1: *reinterpret_cast< bool*>(_v) = isWarningIcon(); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::WriteProperty) {
        _id -= 2;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void AppMediaPlayer_Video_SModel_TextButtonPopup::sTextChanged(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AppMediaPlayer_Video_SModel_TextButtonPopup::isWarningIconChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
