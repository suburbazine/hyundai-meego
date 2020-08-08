/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_PndrFrameTxRx.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_PndrFrameTxRx.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_PndrFrameTxRx.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CPndrFrameTxRx[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      24,   16,   15,   15, 0x05,
      76,   49,   15,   15, 0x05,
     122,  114,   15,   15, 0x05,
     154,   15,   15,   15, 0x05,
     187,  174,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
     213,   16,   15,   15, 0x0a,
     273,  248,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CPndrFrameTxRx[] = {
    "CPndrFrameTxRx\0\0inFrame\0"
    "DataReceived(QByteArray)\0"
    "inDeviceType,inIsConnected\0"
    "DeviceStatusUpdated(EDeviceType,bool)\0"
    "success\0ApplicationLaunched(EPndrError)\0"
    "PndrSessionClosed()\0inSampleRate\0"
    "AudioSamplingChanged(int)\0"
    "EmitDataReceivedSignal(QByteArray)\0"
    "inDeviceType,isConnected\0"
    "EmitDeviceState(EDeviceType,bool)\0"
};

void CPndrFrameTxRx::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CPndrFrameTxRx *_t = static_cast<CPndrFrameTxRx *>(_o);
        switch (_id) {
        case 0: _t->DataReceived((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 1: _t->DeviceStatusUpdated((*reinterpret_cast< const EDeviceType(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 2: _t->ApplicationLaunched((*reinterpret_cast< EPndrError(*)>(_a[1]))); break;
        case 3: _t->PndrSessionClosed(); break;
        case 4: _t->AudioSamplingChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->EmitDataReceivedSignal((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 6: _t->EmitDeviceState((*reinterpret_cast< const EDeviceType(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CPndrFrameTxRx::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CPndrFrameTxRx::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CPndrFrameTxRx,
      qt_meta_data_CPndrFrameTxRx, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CPndrFrameTxRx::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CPndrFrameTxRx::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CPndrFrameTxRx::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CPndrFrameTxRx))
        return static_cast<void*>(const_cast< CPndrFrameTxRx*>(this));
    return QObject::qt_metacast(_clname);
}

int CPndrFrameTxRx::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void CPndrFrameTxRx::DataReceived(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CPndrFrameTxRx::DeviceStatusUpdated(const EDeviceType _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CPndrFrameTxRx::ApplicationLaunched(EPndrError _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CPndrFrameTxRx::PndrSessionClosed()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void CPndrFrameTxRx::AudioSamplingChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
