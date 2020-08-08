/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_PndrBTDBusInterface.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_PndrBTDBusInterface.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_PndrBTDBusInterface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CPandoraBTDBusInterface[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      33,   25,   24,   24, 0x05,
      52,   25,   24,   24, 0x05,
      74,   24,   24,   24, 0x05,
     112,  106,   24,   24, 0x05,

 // slots: signature, parameters, type, tag, flags
     160,   24,  140,   24, 0x0a,
     173,   24,  140,   24, 0x0a,
     198,  189,  140,   24, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CPandoraBTDBusInterface[] = {
    "CPandoraBTDBusInterface\0\0bResult\0"
    "SPPConnected(bool)\0SPPDisconnected(bool)\0"
    "SPPDataReadComplete(QByteArray)\0error\0"
    "SPPDataWriteToBTStatus(int)\0"
    "QDBusPendingReply<>\0SPPConnect()\0"
    "SPPDisconnect()\0baTxData\0"
    "SPPDataWriteToBT(QByteArray)\0"
};

void CPandoraBTDBusInterface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CPandoraBTDBusInterface *_t = static_cast<CPandoraBTDBusInterface *>(_o);
        switch (_id) {
        case 0: _t->SPPConnected((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->SPPDisconnected((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->SPPDataReadComplete((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 3: _t->SPPDataWriteToBTStatus((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 4: { QDBusPendingReply<> _r = _t->SPPConnect();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 5: { QDBusPendingReply<> _r = _t->SPPDisconnect();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 6: { QDBusPendingReply<> _r = _t->SPPDataWriteToBT((*reinterpret_cast< const QByteArray(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CPandoraBTDBusInterface::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CPandoraBTDBusInterface::staticMetaObject = {
    { &QDBusAbstractInterface::staticMetaObject, qt_meta_stringdata_CPandoraBTDBusInterface,
      qt_meta_data_CPandoraBTDBusInterface, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CPandoraBTDBusInterface::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CPandoraBTDBusInterface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CPandoraBTDBusInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CPandoraBTDBusInterface))
        return static_cast<void*>(const_cast< CPandoraBTDBusInterface*>(this));
    return QDBusAbstractInterface::qt_metacast(_clname);
}

int CPandoraBTDBusInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractInterface::qt_metacall(_c, _id, _a);
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
void CPandoraBTDBusInterface::SPPConnected(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CPandoraBTDBusInterface::SPPDisconnected(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CPandoraBTDBusInterface::SPPDataReadComplete(const QByteArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CPandoraBTDBusInterface::SPPDataWriteToBTStatus(const int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
