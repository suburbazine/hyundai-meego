/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppHomeScreen_AppData.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppHomeScreen_AppData.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppHomeScreen_AppData.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppData[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,    9,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      70,   60,    8,    8, 0x0a,

 // methods: signature, parameters, type, tag, flags
     113,    8,  108,    8, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_AppData[] = {
    "AppData\0\0nID,data\0"
    "ValueChanged(HSDefP::APP_DATA_T,QVariant)\0"
    "nId,value\0SetValue(HSDefP::APP_DATA_T,QVariant)\0"
    "bool\0GetParkingMode()\0"
};

void AppData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppData *_t = static_cast<AppData *>(_o);
        switch (_id) {
        case 0: _t->ValueChanged((*reinterpret_cast< HSDefP::APP_DATA_T(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2]))); break;
        case 1: _t->SetValue((*reinterpret_cast< HSDefP::APP_DATA_T(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2]))); break;
        case 2: { bool _r = _t->GetParkingMode();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppData::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppData::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AppData,
      qt_meta_data_AppData, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppData::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppData::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppData))
        return static_cast<void*>(const_cast< AppData*>(this));
    return QObject::qt_metacast(_clname);
}

int AppData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void AppData::ValueChanged(HSDefP::APP_DATA_T _t1, QVariant _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
