/****************************************************************************
** Meta object code from reading C++ file 'dab_adaptor.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "include/adaptors/dab_adaptor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dab_adaptor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DABAdaptor[] = {

 // content:
       6,       // revision
       0,       // classname
       2,   14, // classinfo
       3,   18, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // classinfo: key, value
      33,   11,
     519,   49,

 // signals: signature, parameters, type, tag, flags
     544,  540,  539,  539, 0x05,

 // slots: signature, parameters, type, tag, flags
     586,  540,  539,  539, 0x0a,
     623,  540,  539,  539, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DABAdaptor[] = {
    "DABAdaptor\0com.lge.car.micom.dab\0"
    "D-Bus Interface\0"
    "  <interface name=\"com.lge.car.micom.dab\">\n    <method name=\"LgeDB"
    "us_DABPICodeRequest\">\n      <!-- 0x0126, 20130318(out:2bytes) -->\n "
    "     <arg direction=\"in\" type=\"ay\"/>\n    </method>\n    <method n"
    "ame=\"LgeDBus_DABTAStatus\">\n      <!-- 0x0135, 20130326 -->\n      <"
    "arg direction=\"in\" type=\"y\"/>\n    </method>\n    <signal name=\"L"
    "geDBus_EventDABPISensitivity\">\n      <arg direction=\"out\" type=\"a"
    "y\"/>\n      <!-- 0x8328, 20130318(out:4bytes) -->\n    </signal>\n  <"
    "/interface>\n\0"
    "D-Bus Introspection\0\0in0\0"
    "LgeDBus_EventDABPISensitivity(QByteArray)\0"
    "LgeDBus_DABPICodeRequest(QByteArray)\0"
    "LgeDBus_DABTAStatus(uchar)\0"
};

void DABAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DABAdaptor *_t = static_cast<DABAdaptor *>(_o);
        switch (_id) {
        case 0: _t->LgeDBus_EventDABPISensitivity((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 1: _t->LgeDBus_DABPICodeRequest((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 2: _t->LgeDBus_DABTAStatus((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DABAdaptor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DABAdaptor::staticMetaObject = {
    { &QDBusAbstractAdaptor::staticMetaObject, qt_meta_stringdata_DABAdaptor,
      qt_meta_data_DABAdaptor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DABAdaptor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DABAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DABAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DABAdaptor))
        return static_cast<void*>(const_cast< DABAdaptor*>(this));
    return QDBusAbstractAdaptor::qt_metacast(_clname);
}

int DABAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractAdaptor::qt_metacall(_c, _id, _a);
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
void DABAdaptor::LgeDBus_EventDABPISensitivity(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
