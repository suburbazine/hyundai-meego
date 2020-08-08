/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_IPodController_IAPPacketReader.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "IAPPacketWR/include/DHAVN_IPodController_IAPPacketReader.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_IPodController_IAPPacketReader.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CIAPPacketReader[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      50,   18,   17,   17, 0x05,
      86,   18,   17,   17, 0x05,
     127,   18,   17,   17, 0x05,
     183,   18,   17,   17, 0x05,
     242,  235,   17,   17, 0x05,
     264,   17,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
     302,  278,   17,   17, 0x0a,
     337,   17,   17,   17, 0x0a,
     358,   17,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CIAPPacketReader[] = {
    "CIAPPacketReader\0\0lingoId,commandId,iAPPacketData\0"
    "packetAvailable(int,int,QByteArray)\0"
    "largePacketAvailable(int,int,QByteArray)\0"
    "packetAvailableForPlayerTaskHandler(int,int,QByteArray)\0"
    "packetAvailableForDBTaskHandler(int,int,QByteArray)\0"
    "status\0iAPReaderStatus(bool)\0requestQuit()\0"
    "iAPPacket,isLargePacket\0"
    "iAPPacketAvalable(QByteArray,bool)\0"
    "onDeviceDisconnect()\0closeFileDescriptorSlot()\0"
};

void CIAPPacketReader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CIAPPacketReader *_t = static_cast<CIAPPacketReader *>(_o);
        switch (_id) {
        case 0: _t->packetAvailable((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QByteArray(*)>(_a[3]))); break;
        case 1: _t->largePacketAvailable((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QByteArray(*)>(_a[3]))); break;
        case 2: _t->packetAvailableForPlayerTaskHandler((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QByteArray(*)>(_a[3]))); break;
        case 3: _t->packetAvailableForDBTaskHandler((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QByteArray(*)>(_a[3]))); break;
        case 4: _t->iAPReaderStatus((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->requestQuit(); break;
        case 6: _t->iAPPacketAvalable((*reinterpret_cast< QByteArray(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 7: _t->onDeviceDisconnect(); break;
        case 8: _t->closeFileDescriptorSlot(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CIAPPacketReader::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CIAPPacketReader::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CIAPPacketReader,
      qt_meta_data_CIAPPacketReader, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CIAPPacketReader::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CIAPPacketReader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CIAPPacketReader::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CIAPPacketReader))
        return static_cast<void*>(const_cast< CIAPPacketReader*>(this));
    return QObject::qt_metacast(_clname);
}

int CIAPPacketReader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void CIAPPacketReader::packetAvailable(int _t1, int _t2, QByteArray _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CIAPPacketReader::largePacketAvailable(int _t1, int _t2, QByteArray _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CIAPPacketReader::packetAvailableForPlayerTaskHandler(int _t1, int _t2, QByteArray _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CIAPPacketReader::packetAvailableForDBTaskHandler(int _t1, int _t2, QByteArray _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CIAPPacketReader::iAPReaderStatus(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CIAPPacketReader::requestQuit()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}
QT_END_MOC_NAMESPACE
