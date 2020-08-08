/****************************************************************************
** Meta object code from reading C++ file 'system.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "include/objects/system.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'system.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_System[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,    8,    7,    7, 0x05,
      48,    8,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
      84,    7,    7,    7, 0x0a,
     123,    7,  112,    7, 0x0a,
     144,    7,  112,    7, 0x0a,
     167,    8,  112,    7, 0x0a,
     198,    7,    7,    7, 0x0a,
     222,    8,    7,    7, 0x0a,
     253,    8,    7,    7, 0x0a,
     285,    7,    7,    7, 0x0a,
     317,    7,    7,    7, 0x0a,
     348,    7,    7,    7, 0x0a,
     377,    7,    7,    7, 0x0a,
     405,    8,    7,    7, 0x0a,
     434,    8,    7,    7, 0x0a,
     466,    8,    7,    7, 0x0a,
     500,    8,    7,    7, 0x0a,
     536,    8,    7,    7, 0x0a,
     574,    8,    7,    7, 0x0a,
     606,    8,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_System[] = {
    "System\0\0in0\0LgeDBus_EventFrontSpecialKey(uchar)\0"
    "LgeDBus_FactoryResetComplete(uchar)\0"
    "LgeDBus_AVMInitialRequest()\0QByteArray\0"
    "LgeDBus_AskRTCInfo()\0LgeDBus_WDTCountInfo()\0"
    "LgeDBus_EEPROMRead(QByteArray)\0"
    "LgeDBus_WDTCountClear()\0"
    "LgeDBus_AppBootComplete(uchar)\0"
    "LgeDBus_EEPROMWrite(QByteArray)\0"
    "LgeDBus_CarInfoInitialRequest()\0"
    "LgeDBus_CarSetInitialRequest()\0"
    "LgeDBus_DATCInitialRequest()\0"
    "LgeDBus_PGSInitialRequest()\0"
    "LgeDBus_SetFanControl(uchar)\0"
    "LgeDBus_SetHwRTCSet(QByteArray)\0"
    "LgeDBus_SetPhoneMicControl(uchar)\0"
    "LgeDBus_SetDiscIndicationLed(uchar)\0"
    "LgeDBus_ResetHWPeripheral(QByteArray)\0"
    "LgeDBus_MainImageUpgrade(uchar)\0"
    "LgeDBus_ETCFactorySetting(uchar)\0"
};

void System::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        System *_t = static_cast<System *>(_o);
        switch (_id) {
        case 0: _t->LgeDBus_EventFrontSpecialKey((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 1: _t->LgeDBus_FactoryResetComplete((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 2: _t->LgeDBus_AVMInitialRequest(); break;
        case 3: { QByteArray _r = _t->LgeDBus_AskRTCInfo();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 4: { QByteArray _r = _t->LgeDBus_WDTCountInfo();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 5: { QByteArray _r = _t->LgeDBus_EEPROMRead((*reinterpret_cast< const QByteArray(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 6: _t->LgeDBus_WDTCountClear(); break;
        case 7: _t->LgeDBus_AppBootComplete((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 8: _t->LgeDBus_EEPROMWrite((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 9: _t->LgeDBus_CarInfoInitialRequest(); break;
        case 10: _t->LgeDBus_CarSetInitialRequest(); break;
        case 11: _t->LgeDBus_DATCInitialRequest(); break;
        case 12: _t->LgeDBus_PGSInitialRequest(); break;
        case 13: _t->LgeDBus_SetFanControl((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 14: _t->LgeDBus_SetHwRTCSet((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 15: _t->LgeDBus_SetPhoneMicControl((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 16: _t->LgeDBus_SetDiscIndicationLed((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 17: _t->LgeDBus_ResetHWPeripheral((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 18: _t->LgeDBus_MainImageUpgrade((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 19: _t->LgeDBus_ETCFactorySetting((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData System::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject System::staticMetaObject = {
    { &ObjectAbstractInterface::staticMetaObject, qt_meta_stringdata_System,
      qt_meta_data_System, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &System::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *System::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *System::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_System))
        return static_cast<void*>(const_cast< System*>(this));
    if (!strcmp(_clname, "QDBusContext"))
        return static_cast< QDBusContext*>(const_cast< System*>(this));
    return ObjectAbstractInterface::qt_metacast(_clname);
}

int System::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ObjectAbstractInterface::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    }
    return _id;
}

// SIGNAL 0
void System::LgeDBus_EventFrontSpecialKey(uchar _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void System::LgeDBus_FactoryResetComplete(uchar _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
