/****************************************************************************
** Meta object code from reading C++ file 'tmc.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "include/objects/tmc.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tmc.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Tmc[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
       5,    4,    4,    4, 0x05,
      41,    4,    4,    4, 0x05,

 // slots: signature, parameters, type, tag, flags
      88,   80,    4,    4, 0x0a,
     133,  129,    4,    4, 0x0a,
     194,    4,  183,    4, 0x0a,
     219,    4,  183,    4, 0x0a,
     244,    4,  183,    4, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Tmc[] = {
    "Tmc\0\0LgeDBus_EventInfoUpdate(QByteArray)\0"
    "LgeDBus_EventRdsFreqUpdate(QByteArray)\0"
    "in0,in1\0LgeDBus_SetManualFreqChange(uchar,uchar)\0"
    "in0\0LgeDBus_SetCountryCodeAndDecodeStatus(QByteArray)\0"
    "QByteArray\0LgeDBus_AskCurrentFreq()\0"
    "LgeDBus_AskStationList()\0"
    "LgeDBus_AskStationSensitivity()\0"
};

void Tmc::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Tmc *_t = static_cast<Tmc *>(_o);
        switch (_id) {
        case 0: _t->LgeDBus_EventInfoUpdate((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 1: _t->LgeDBus_EventRdsFreqUpdate((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 2: _t->LgeDBus_SetManualFreqChange((*reinterpret_cast< uchar(*)>(_a[1])),(*reinterpret_cast< uchar(*)>(_a[2]))); break;
        case 3: _t->LgeDBus_SetCountryCodeAndDecodeStatus((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 4: { QByteArray _r = _t->LgeDBus_AskCurrentFreq();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 5: { QByteArray _r = _t->LgeDBus_AskStationList();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 6: { QByteArray _r = _t->LgeDBus_AskStationSensitivity();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Tmc::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Tmc::staticMetaObject = {
    { &ObjectAbstractInterface::staticMetaObject, qt_meta_stringdata_Tmc,
      qt_meta_data_Tmc, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Tmc::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Tmc::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Tmc::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Tmc))
        return static_cast<void*>(const_cast< Tmc*>(this));
    if (!strcmp(_clname, "QDBusContext"))
        return static_cast< QDBusContext*>(const_cast< Tmc*>(this));
    return ObjectAbstractInterface::qt_metacast(_clname);
}

int Tmc::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ObjectAbstractInterface::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void Tmc::LgeDBus_EventInfoUpdate(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Tmc::LgeDBus_EventRdsFreqUpdate(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
