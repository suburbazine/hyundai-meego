/****************************************************************************
** Meta object code from reading C++ file 'packetbuffer.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/devicemanager/packetbuffer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'packetbuffer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PacketBuffer[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,
      33,   13,   13,   13, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_PacketBuffer[] = {
    "PacketBuffer\0\0NewPacketArrived()\0"
    "SyncPacketArrived()\0"
};

void PacketBuffer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PacketBuffer *_t = static_cast<PacketBuffer *>(_o);
        switch (_id) {
        case 0: _t->NewPacketArrived(); break;
        case 1: _t->SyncPacketArrived(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData PacketBuffer::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PacketBuffer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_PacketBuffer,
      qt_meta_data_PacketBuffer, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PacketBuffer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PacketBuffer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PacketBuffer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PacketBuffer))
        return static_cast<void*>(const_cast< PacketBuffer*>(this));
    return QObject::qt_metacast(_clname);
}

int PacketBuffer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void PacketBuffer::NewPacketArrived()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void PacketBuffer::SyncPacketArrived()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
