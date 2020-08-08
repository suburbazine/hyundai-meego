/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_MOSTManager_BluetoothFblockControl.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_MOSTManager_BluetoothFblockControl.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_MOSTManager_BluetoothFblockControl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CBluetoothFblockControl[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      54,   25,   24,   24, 0x05,
     118,   86,   24,   24, 0x05,
     175,  159,   24,   24, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_CBluetoothFblockControl[] = {
    "CBluetoothFblockControl\0\0"
    "senderHandle,callId,deviceID\0"
    "StartHangupCall(uint,uint,uint)\0"
    "senderHandle,telNumber,deviceID\0"
    "DialNumberRequestIBOX(uint,QString,uint)\0"
    "usermode,micOff\0UserMode(uint,bool)\0"
};

void CBluetoothFblockControl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CBluetoothFblockControl *_t = static_cast<CBluetoothFblockControl *>(_o);
        switch (_id) {
        case 0: _t->StartHangupCall((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3]))); break;
        case 1: _t->DialNumberRequestIBOX((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3]))); break;
        case 2: _t->UserMode((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const bool(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CBluetoothFblockControl::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CBluetoothFblockControl::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CBluetoothFblockControl,
      qt_meta_data_CBluetoothFblockControl, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CBluetoothFblockControl::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CBluetoothFblockControl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CBluetoothFblockControl::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CBluetoothFblockControl))
        return static_cast<void*>(const_cast< CBluetoothFblockControl*>(this));
    if (!strcmp(_clname, "CControlThreadBase"))
        return static_cast< CControlThreadBase*>(const_cast< CBluetoothFblockControl*>(this));
    return QObject::qt_metacast(_clname);
}

int CBluetoothFblockControl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void CBluetoothFblockControl::StartHangupCall(const uint _t1, const uint _t2, const uint _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CBluetoothFblockControl::DialNumberRequestIBOX(const uint _t1, const QString _t2, const uint _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CBluetoothFblockControl::UserMode(const uint _t1, const bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
