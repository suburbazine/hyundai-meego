/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_PndrBTFrameTxRx.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_PndrBTFrameTxRx.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_PndrBTFrameTxRx.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CPndrBTFrameTxRx[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      26,   18,   17,   17, 0x05,
      80,   53,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
     130,  120,   17,   17, 0x0a,
     164,  151,   17,   17, 0x0a,
     188,   17,   17,   17, 0x0a,
     228,  222,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CPndrBTFrameTxRx[] = {
    "CPndrBTFrameTxRx\0\0inFrame\0"
    "BTDataReceived(QByteArray)\0"
    "inDeviceType,inIsConnected\0"
    "BTDeviceStatusUpdated(EDeviceType,bool)\0"
    "inConnect\0onSPPConnected(bool)\0"
    "inDisconnect\0onSPPDisconnected(bool)\0"
    "onSPPDataReadComplete(QByteArray)\0"
    "error\0onSPPDataWriteToBTStatus(int)\0"
};

void CPndrBTFrameTxRx::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CPndrBTFrameTxRx *_t = static_cast<CPndrBTFrameTxRx *>(_o);
        switch (_id) {
        case 0: _t->BTDataReceived((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 1: _t->BTDeviceStatusUpdated((*reinterpret_cast< const EDeviceType(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 2: _t->onSPPConnected((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->onSPPDisconnected((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->onSPPDataReadComplete((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 5: _t->onSPPDataWriteToBTStatus((*reinterpret_cast< const int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CPndrBTFrameTxRx::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CPndrBTFrameTxRx::staticMetaObject = {
    { &CPndrFrameTxRx::staticMetaObject, qt_meta_stringdata_CPndrBTFrameTxRx,
      qt_meta_data_CPndrBTFrameTxRx, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CPndrBTFrameTxRx::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CPndrBTFrameTxRx::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CPndrBTFrameTxRx::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CPndrBTFrameTxRx))
        return static_cast<void*>(const_cast< CPndrBTFrameTxRx*>(this));
    return CPndrFrameTxRx::qt_metacast(_clname);
}

int CPndrBTFrameTxRx::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CPndrFrameTxRx::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void CPndrBTFrameTxRx::BTDataReceived(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CPndrBTFrameTxRx::BTDeviceStatusUpdated(const EDeviceType _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
