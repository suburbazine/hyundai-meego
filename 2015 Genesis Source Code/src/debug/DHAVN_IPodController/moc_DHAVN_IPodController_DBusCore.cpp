/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_IPodController_DBusCore.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "DBus/include/DHAVN_IPodController_DBusCore.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_IPodController_DBusCore.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CIPodController_DBus[] = {

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
      39,   22,   21,   21, 0x05,
      88,   77,   21,   21, 0x05,

 // slots: signature, parameters, type, tag, flags
     122,   77,  114,   21, 0x0a,
     155,  149,  142,   21, 0x0a,
     188,   22,   21,   21, 0x08,
     228,   77,   21,   21, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CIPodController_DBus[] = {
    "CIPodController_DBus\0\0ipodNumber,appId\0"
    "IpodAppSessionClosed(QString,QString)\0"
    "ipodNumber\0IPodDisconnected(QString)\0"
    "QString\0GetHidPath(QString)\0qint16\0"
    "AppId\0GetApplicationSessionID(QString)\0"
    "OnIpodAppSessionClosed(QString,QString)\0"
    "OnIPodDisconnected(QString)\0"
};

void CIPodController_DBus::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CIPodController_DBus *_t = static_cast<CIPodController_DBus *>(_o);
        switch (_id) {
        case 0: _t->IpodAppSessionClosed((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: _t->IPodDisconnected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: { QString _r = _t->GetHidPath((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 3: { qint16 _r = _t->GetApplicationSessionID((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< qint16*>(_a[0]) = _r; }  break;
        case 4: _t->OnIpodAppSessionClosed((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 5: _t->OnIPodDisconnected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CIPodController_DBus::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CIPodController_DBus::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CIPodController_DBus,
      qt_meta_data_CIPodController_DBus, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CIPodController_DBus::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CIPodController_DBus::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CIPodController_DBus::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CIPodController_DBus))
        return static_cast<void*>(const_cast< CIPodController_DBus*>(this));
    return QObject::qt_metacast(_clname);
}

int CIPodController_DBus::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void CIPodController_DBus::IpodAppSessionClosed(const QString & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CIPodController_DBus::IPodDisconnected(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
