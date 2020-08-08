/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_IPodController_DBusAdaptor.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "DBus/include/DHAVN_IPodController_DBusAdaptor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_IPodController_DBusAdaptor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DbusTestAdaptor[] = {

 // content:
       6,       // revision
       0,       // classname
       2,   14, // classinfo
       4,   18, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // classinfo: key, value
      33,   16,
     627,   49,

 // signals: signature, parameters, type, tag, flags
     659,  648,  647,  647, 0x05,
     702,  685,  647,  647, 0x05,

 // slots: signature, parameters, type, tag, flags
     753,  747,  740,  647, 0x0a,
     794,  648,  786,  647, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DbusTestAdaptor[] = {
    "DbusTestAdaptor\0org.lge.dbusTest\0"
    "D-Bus Interface\0"
    "  <interface name=\"org.lge.dbusTest\">\n    <signal name=\"IpodAppSes"
    "sionClosed\">\n      <arg direction=\"in\" type=\"s\" name=\"AppId\"/>"
    "\n    </signal>\n    <signal name=\"IPodDisconnected\">\n      <arg di"
    "rection=\"in\" type=\"s\" name=\"ipodNumber\"/>\n    </signal>\n    <m"
    "ethod name=\"GetHidPath\">\n      <arg direction=\"in\" type=\"s\" nam"
    "e=\"ipodNumber\"/>\n      <arg direction=\"out\" type=\"s\" name=\"pat"
    "h\"/>\n    </method>\n    <method name=\"GetApplicationSessionID\">\n "
    "     <arg direction=\"in\" type=\"s\" name=\"AppId\"/>\n      <arg dir"
    "ection=\"out\" type=\"i\" name=\"SessionID\"/>\n    </method>\n  </int"
    "erface>\n\0"
    "D-Bus Introspection\0\0ipodNumber\0"
    "IPodDisconnected(QString)\0ipodNumber,appId\0"
    "IpodAppSessionClosed(QString,QString)\0"
    "qint16\0AppId\0GetApplicationSessionID(QString)\0"
    "QString\0GetHidPath(QString)\0"
};

void DbusTestAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DbusTestAdaptor *_t = static_cast<DbusTestAdaptor *>(_o);
        switch (_id) {
        case 0: _t->IPodDisconnected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->IpodAppSessionClosed((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: { qint16 _r = _t->GetApplicationSessionID((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< qint16*>(_a[0]) = _r; }  break;
        case 3: { QString _r = _t->GetHidPath((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DbusTestAdaptor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DbusTestAdaptor::staticMetaObject = {
    { &QDBusAbstractAdaptor::staticMetaObject, qt_meta_stringdata_DbusTestAdaptor,
      qt_meta_data_DbusTestAdaptor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DbusTestAdaptor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DbusTestAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DbusTestAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DbusTestAdaptor))
        return static_cast<void*>(const_cast< DbusTestAdaptor*>(this));
    return QDBusAbstractAdaptor::qt_metacast(_clname);
}

int DbusTestAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractAdaptor::qt_metacall(_c, _id, _a);
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
void DbusTestAdaptor::IPodDisconnected(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DbusTestAdaptor::IpodAppSessionClosed(const QString & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
