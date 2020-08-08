/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_IPodAppController_IAPPacketReader.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../IAPPacketWR/include/DHAVN_IPodAppController_IAPPacketReader.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_IPodAppController_IAPPacketReader.h' doesn't include <QObject>."
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
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      50,   18,   17,   17, 0x05,
      87,   18,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
     139,  129,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CIAPPacketReader[] = {
    "CIAPPacketReader\0\0lingoId,commandId,iAPPacketData\0"
    "packetAvailable(int,int,QByteArray&)\0"
    "largePacketAvailable(int,int,QByteArray&)\0"
    "hidPacket\0hidReportAvalable(QByteArray)\0"
};

void CIAPPacketReader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CIAPPacketReader *_t = static_cast<CIAPPacketReader *>(_o);
        switch (_id) {
        case 0: _t->packetAvailable((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QByteArray(*)>(_a[3]))); break;
        case 1: _t->largePacketAvailable((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QByteArray(*)>(_a[3]))); break;
        case 2: _t->hidReportAvalable((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
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
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void CIAPPacketReader::packetAvailable(int _t1, int _t2, QByteArray & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CIAPPacketReader::largePacketAvailable(int _t1, int _t2, QByteArray & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
