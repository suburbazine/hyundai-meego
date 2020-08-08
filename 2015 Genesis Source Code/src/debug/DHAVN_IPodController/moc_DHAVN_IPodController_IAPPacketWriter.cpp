/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_IPodController_IAPPacketWriter.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "IAPPacketWR/include/DHAVN_IPodController_IAPPacketWriter.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_IPodController_IAPPacketWriter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CIAPPacketWriter[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      25,   18,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      47,   17,   17,   17, 0x0a,
      80,   68,   17,   17, 0x0a,
     106,   18,   17,   17, 0x0a,
     125,   17,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CIAPPacketWriter[] = {
    "CIAPPacketWriter\0\0status\0iAPWriterStatus(bool)\0"
    "onDeviceDisconnect()\0packetBytes\0"
    "sendiAPPacket(QByteArray)\0onAuthorised(bool)\0"
    "closeFileDescriptorSlot()\0"
};

void CIAPPacketWriter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CIAPPacketWriter *_t = static_cast<CIAPPacketWriter *>(_o);
        switch (_id) {
        case 0: _t->iAPWriterStatus((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->onDeviceDisconnect(); break;
        case 2: _t->sendiAPPacket((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 3: _t->onAuthorised((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->closeFileDescriptorSlot(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CIAPPacketWriter::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CIAPPacketWriter::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CIAPPacketWriter,
      qt_meta_data_CIAPPacketWriter, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CIAPPacketWriter::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CIAPPacketWriter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CIAPPacketWriter::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CIAPPacketWriter))
        return static_cast<void*>(const_cast< CIAPPacketWriter*>(this));
    return QObject::qt_metacast(_clname);
}

int CIAPPacketWriter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void CIAPPacketWriter::iAPWriterStatus(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
