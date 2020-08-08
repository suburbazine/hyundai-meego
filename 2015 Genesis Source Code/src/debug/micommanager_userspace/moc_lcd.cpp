/****************************************************************************
** Meta object code from reading C++ file 'lcd.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "include/objects/lcd.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'lcd.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Lcd[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
       9,    5,    4,    4, 0x05,

 // slots: signature, parameters, type, tag, flags
      58,    4,   47,    4, 0x0a,
      87,    5,    4,    4, 0x0a,
     138,  122,    4,    4, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Lcd[] = {
    "Lcd\0\0in0\0LgeDBus_EventLCDSetupInfo(QByteArray)\0"
    "QByteArray\0LgeDBus_AskLcdSetInfoValue()\0"
    "LgeDBus_SetBacklightControl(uchar)\0"
    "in0,in1,in2,in3\0"
    "LgeDBus_SetLcdPWMControl(uchar,uchar,uchar,uchar)\0"
};

void Lcd::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Lcd *_t = static_cast<Lcd *>(_o);
        switch (_id) {
        case 0: _t->LgeDBus_EventLCDSetupInfo((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 1: { QByteArray _r = _t->LgeDBus_AskLcdSetInfoValue();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 2: _t->LgeDBus_SetBacklightControl((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 3: _t->LgeDBus_SetLcdPWMControl((*reinterpret_cast< uchar(*)>(_a[1])),(*reinterpret_cast< uchar(*)>(_a[2])),(*reinterpret_cast< uchar(*)>(_a[3])),(*reinterpret_cast< uchar(*)>(_a[4]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Lcd::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Lcd::staticMetaObject = {
    { &ObjectAbstractInterface::staticMetaObject, qt_meta_stringdata_Lcd,
      qt_meta_data_Lcd, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Lcd::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Lcd::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Lcd::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Lcd))
        return static_cast<void*>(const_cast< Lcd*>(this));
    if (!strcmp(_clname, "QDBusContext"))
        return static_cast< QDBusContext*>(const_cast< Lcd*>(this));
    return ObjectAbstractInterface::qt_metacast(_clname);
}

int Lcd::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ObjectAbstractInterface::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void Lcd::LgeDBus_EventLCDSetupInfo(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
