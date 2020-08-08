/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_IPodAppController_hidRawPacketHandler.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../hidRawPacketHandler/include/DHAVN_IPodAppController_hidRawPacketHandler.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_IPodAppController_hidRawPacketHandler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CHIDRawPacketHandle[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      31,   21,   20,   20, 0x05,
      69,   61,   20,   20, 0x05,

 // slots: signature, parameters, type, tag, flags
     100,   93,   20,   20, 0x0a,
     114,   61,   20,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CHIDRawPacketHandle[] = {
    "CHIDRawPacketHandle\0\0hidPacket\0"
    "hidPacketReceived(QByteArray)\0hidData\0"
    "emitToWrite(QByteArray)\0socket\0"
    "Notified(int)\0writeToSock(QByteArray)\0"
};

void CHIDRawPacketHandle::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CHIDRawPacketHandle *_t = static_cast<CHIDRawPacketHandle *>(_o);
        switch (_id) {
        case 0: _t->hidPacketReceived((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 1: _t->emitToWrite((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 2: _t->Notified((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->writeToSock((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CHIDRawPacketHandle::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CHIDRawPacketHandle::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CHIDRawPacketHandle,
      qt_meta_data_CHIDRawPacketHandle, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CHIDRawPacketHandle::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CHIDRawPacketHandle::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CHIDRawPacketHandle::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CHIDRawPacketHandle))
        return static_cast<void*>(const_cast< CHIDRawPacketHandle*>(this));
    return QObject::qt_metacast(_clname);
}

int CHIDRawPacketHandle::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void CHIDRawPacketHandle::hidPacketReceived(QByteArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CHIDRawPacketHandle::emitToWrite(QByteArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
