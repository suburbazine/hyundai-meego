/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppFileManager_TitleProvider.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppFileManager_TitleProvider.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppFileManager_TitleProvider.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TitleProvider[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       3,   24, // properties
       2,   36, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      34,   14,   14,   14, 0x08,

 // properties: name, type, flags
      51,   43, 0x0a495001,
      65,   61, 0x02095003,
      82,   76, 0x40095103,

 // properties: notify_signal_id
       0,
       0,
       0,

 // enums: name, flags, count, data
      87, 0x0,    2,   44,
      99, 0x0,    2,   48,

 // enum data: key, value
     118, uint(TitleProvider::TITLE_TYPE_CURRENT_PATH),
     142, uint(TitleProvider::TITLE_TYPE_KEYPAD),
     160, uint(TitleProvider::CURRENT_PATH_FOLDER),
     180, uint(TitleProvider::CURRENT_PATH_FILE),

       0        // eod
};

static const char qt_meta_stringdata_TitleProvider[] = {
    "TitleProvider\0\0titleTextChanged()\0"
    "update()\0QString\0titleText\0int\0"
    "titleWidth\0QFont\0font\0TITLE_TYPES\0"
    "CURRENT_PATH_TYPES\0TITLE_TYPE_CURRENT_PATH\0"
    "TITLE_TYPE_KEYPAD\0CURRENT_PATH_FOLDER\0"
    "CURRENT_PATH_FILE\0"
};

void TitleProvider::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        TitleProvider *_t = static_cast<TitleProvider *>(_o);
        switch (_id) {
        case 0: _t->titleTextChanged(); break;
        case 1: _t->update(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData TitleProvider::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TitleProvider::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TitleProvider,
      qt_meta_data_TitleProvider, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TitleProvider::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TitleProvider::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TitleProvider::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TitleProvider))
        return static_cast<void*>(const_cast< TitleProvider*>(this));
    return QObject::qt_metacast(_clname);
}

int TitleProvider::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
        case 0: *reinterpret_cast< QString*>(_v) = getTitleText(); break;
        case 1: *reinterpret_cast< int*>(_v) = getTitleWidth(); break;
        case 2: *reinterpret_cast< QFont*>(_v) = getFont(); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 1: setTitltWidth(*reinterpret_cast< int*>(_v)); break;
        case 2: setFont(*reinterpret_cast< QFont*>(_v)); break;
        }
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
void TitleProvider::titleTextChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
