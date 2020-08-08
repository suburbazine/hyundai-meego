/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_PndrUSBFrameTxRx.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_PndrUSBFrameTxRx.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_PndrUSBFrameTxRx.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CPndrUSBFrameTxRx[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      27,   19,   18,   18, 0x05,
      82,   55,   18,   18, 0x05,
     131,  123,   18,   18, 0x05,
     163,   18,   18,   18, 0x05,
     194,  183,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
     220,   18,   18,   18, 0x0a,
     266,  256,   18,   18, 0x0a,
     300,  293,   18,   18, 0x0a,
     340,  334,   18,   18, 0x0a,
     383,  372,   18,   18, 0x0a,
     409,   18,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CPndrUSBFrameTxRx[] = {
    "CPndrUSBFrameTxRx\0\0inFrame\0"
    "USBDataReceived(QByteArray)\0"
    "inDeviceType,inIsConnected\0"
    "USBDeviceStatusUpdated(EDeviceType,bool)\0"
    "success\0ApplicationLaunched(EPndrError)\0"
    "PndrSessionClosed()\0sampleRate\0"
    "AudioSamplingChanged(int)\0"
    "onIPodAppControllerInitCompletion()\0"
    "dataArray\0onDataReceived(QByteArray)\0"
    "status\0onAcknowledgement(EIPodAppErrors)\0"
    "error\0onSessionClosed(EIPodAppErrors)\0"
    "ipodNumber\0onDeviceDisconnected(int)\0"
    "onNotifySessionClosed()\0"
};

void CPndrUSBFrameTxRx::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CPndrUSBFrameTxRx *_t = static_cast<CPndrUSBFrameTxRx *>(_o);
        switch (_id) {
        case 0: _t->USBDataReceived((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 1: _t->USBDeviceStatusUpdated((*reinterpret_cast< const EDeviceType(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 2: _t->ApplicationLaunched((*reinterpret_cast< EPndrError(*)>(_a[1]))); break;
        case 3: _t->PndrSessionClosed(); break;
        case 4: _t->AudioSamplingChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->onIPodAppControllerInitCompletion(); break;
        case 6: _t->onDataReceived((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 7: _t->onAcknowledgement((*reinterpret_cast< EIPodAppErrors(*)>(_a[1]))); break;
        case 8: _t->onSessionClosed((*reinterpret_cast< EIPodAppErrors(*)>(_a[1]))); break;
        case 9: _t->onDeviceDisconnected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->onNotifySessionClosed(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CPndrUSBFrameTxRx::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CPndrUSBFrameTxRx::staticMetaObject = {
    { &CPndrFrameTxRx::staticMetaObject, qt_meta_stringdata_CPndrUSBFrameTxRx,
      qt_meta_data_CPndrUSBFrameTxRx, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CPndrUSBFrameTxRx::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CPndrUSBFrameTxRx::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CPndrUSBFrameTxRx::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CPndrUSBFrameTxRx))
        return static_cast<void*>(const_cast< CPndrUSBFrameTxRx*>(this));
    return CPndrFrameTxRx::qt_metacast(_clname);
}

int CPndrUSBFrameTxRx::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CPndrFrameTxRx::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void CPndrUSBFrameTxRx::USBDataReceived(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CPndrUSBFrameTxRx::USBDeviceStatusUpdated(const EDeviceType _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CPndrUSBFrameTxRx::ApplicationLaunched(EPndrError _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CPndrUSBFrameTxRx::PndrSessionClosed()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void CPndrUSBFrameTxRx::AudioSamplingChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
