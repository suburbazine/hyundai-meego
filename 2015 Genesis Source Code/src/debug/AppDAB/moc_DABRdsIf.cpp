/****************************************************************************
** Meta object code from reading C++ file 'DABRdsIf.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DABRdsIf.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DABRdsIf.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DABRdsIf[] = {

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
      29,    9,
     508,   45,

 // signals: signature, parameters, type, tag, flags
     535,  529,  528,  528, 0x05,
     557,  529,  528,  528, 0x05,

 // slots: signature, parameters, type, tag, flags
     582,  529,  528,  528, 0x0a,
     603,  529,  528,  528, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DABRdsIf[] = {
    "DABRdsIf\0com.lge.car.dab.rds\0"
    "D-Bus Interface\0"
    "  <interface name=\"com.lge.car.dab.rds\">\n    <signal name=\"reqTraf"
    "ficDABToRDS\">\n      <arg direction=\"out\" type=\"b\" name=\"onOff\""
    "/>\n    </signal>\n    <signal name=\"reqNewsDABtoRDS\">\n      <arg d"
    "irection=\"out\" type=\"b\" name=\"onOff\"/>\n    </signal>\n    <meth"
    "od name=\"onTrafficRDStoDAB\">\n      <arg direction=\"in\" type=\"b\""
    " name=\"onOff\"/>\n    </method>\n    <method name=\"onNewsRDStoDAB\">"
    "\n      <arg direction=\"in\" type=\"b\" name=\"onOff\"/>\n    </metho"
    "d>\n  </interface>\n\0"
    "D-Bus Introspection\0\0onOff\0"
    "reqNewsDABtoRDS(bool)\0reqTrafficDABToRDS(bool)\0"
    "onNewsRDStoDAB(bool)\0onTrafficRDStoDAB(bool)\0"
};

void DABRdsIf::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DABRdsIf *_t = static_cast<DABRdsIf *>(_o);
        switch (_id) {
        case 0: _t->reqNewsDABtoRDS((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->reqTrafficDABToRDS((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->onNewsRDStoDAB((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->onTrafficRDStoDAB((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DABRdsIf::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DABRdsIf::staticMetaObject = {
    { &QDBusAbstractAdaptor::staticMetaObject, qt_meta_stringdata_DABRdsIf,
      qt_meta_data_DABRdsIf, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DABRdsIf::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DABRdsIf::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DABRdsIf::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DABRdsIf))
        return static_cast<void*>(const_cast< DABRdsIf*>(this));
    return QDBusAbstractAdaptor::qt_metacast(_clname);
}

int DABRdsIf::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void DABRdsIf::reqNewsDABtoRDS(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DABRdsIf::reqTrafficDABToRDS(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
