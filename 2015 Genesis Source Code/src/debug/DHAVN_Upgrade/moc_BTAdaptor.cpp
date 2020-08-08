/****************************************************************************
** Meta object code from reading C++ file 'BTAdaptor.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/bt/BTAdaptor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BTAdaptor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_BTAdaptor[] = {

 // content:
       6,       // revision
       0,       // classname
       2,   14, // classinfo
       1,   18, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // classinfo: key, value
      32,   10,
     193,   48,

 // signals: signature, parameters, type, tag, flags
     214,  213,  213,  213, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_BTAdaptor[] = {
    "BTAdaptor\0com.lge.car.upgradebt\0"
    "D-Bus Interface\0"
    "  <interface name=\"com.lge.car.upgradebt\">\n    <signal name=\"LgeDB"
    "us_reqBTDisconnect\"/>\n       <arg direction = \"out\" type=\"y\"/>\n"
    "  </interface>\n\0"
    "D-Bus Introspection\0\0LgeDBus_reqBTDisconnect()\0"
};

void BTAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        BTAdaptor *_t = static_cast<BTAdaptor *>(_o);
        switch (_id) {
        case 0: _t->LgeDBus_reqBTDisconnect(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData BTAdaptor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject BTAdaptor::staticMetaObject = {
    { &QDBusAbstractAdaptor::staticMetaObject, qt_meta_stringdata_BTAdaptor,
      qt_meta_data_BTAdaptor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &BTAdaptor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *BTAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *BTAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BTAdaptor))
        return static_cast<void*>(const_cast< BTAdaptor*>(this));
    return QDBusAbstractAdaptor::qt_metacast(_clname);
}

int BTAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractAdaptor::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void BTAdaptor::LgeDBus_reqBTDisconnect()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
