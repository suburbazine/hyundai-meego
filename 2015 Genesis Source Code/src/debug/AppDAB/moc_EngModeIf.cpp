/****************************************************************************
** Meta object code from reading C++ file 'EngModeIf.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/EngModeIf.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'EngModeIf.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_EngModeIf[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   11,   10,   10, 0x05,
      36,   10,   10,   10, 0x05,
      51,   11,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      95,   87,   67,   10, 0x0a,
     121,   87,   67,   10, 0x0a,
     146,   11,   67,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_EngModeIf[] = {
    "EngModeIf\0\0in0\0reqConnectivity(int)\0"
    "reqDiagnosis()\0reqVersion(int)\0"
    "QDBusPendingReply<>\0in0,in1\0"
    "ConnectivityInfo(int,int)\0"
    "VersionInfo(int,QString)\0resDiagnosisEnd(int)\0"
};

void EngModeIf::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        EngModeIf *_t = static_cast<EngModeIf *>(_o);
        switch (_id) {
        case 0: _t->reqConnectivity((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->reqDiagnosis(); break;
        case 2: _t->reqVersion((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: { QDBusPendingReply<> _r = _t->ConnectivityInfo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 4: { QDBusPendingReply<> _r = _t->VersionInfo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 5: { QDBusPendingReply<> _r = _t->resDiagnosisEnd((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData EngModeIf::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject EngModeIf::staticMetaObject = {
    { &QDBusAbstractInterface::staticMetaObject, qt_meta_stringdata_EngModeIf,
      qt_meta_data_EngModeIf, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &EngModeIf::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *EngModeIf::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *EngModeIf::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EngModeIf))
        return static_cast<void*>(const_cast< EngModeIf*>(this));
    return QDBusAbstractInterface::qt_metacast(_clname);
}

int EngModeIf::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractInterface::qt_metacall(_c, _id, _a);
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
void EngModeIf::reqConnectivity(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void EngModeIf::reqDiagnosis()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void EngModeIf::reqVersion(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
