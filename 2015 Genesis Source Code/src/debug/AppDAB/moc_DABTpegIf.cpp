/****************************************************************************
** Meta object code from reading C++ file 'DABTpegIf.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DABTpegIf.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DABTpegIf.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DABTpegIf[] = {

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
      32,   10,
     332,   48,

 // signals: signature, parameters, type, tag, flags
     369,  353,  352,  352, 0x05,

 // slots: signature, parameters, type, tag, flags
     398,  352,  352,  352, 0x0a,
     415,  352,  352,  352, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DABTpegIf[] = {
    "DABTpegIf\0com.lge.car.dhavn.Dmb\0"
    "D-Bus Interface\0"
    "  <interface name=\"com.lge.car.dhavn.Dmb\">\n    <signal name=\"sendT"
    "PEGData\">\n      <arg direction=\"out\" type=\"ay\" name=\"tpegData\""
    "/>\n      <arg direction=\"out\" type=\"i\" name=\"length\"/>\n    </s"
    "ignal>\n    <method name=\"reqTPEGDataOn\"/>\n    <method name=\"reqTP"
    "EGDataOff\"/>\n  </interface>\n\0"
    "D-Bus Introspection\0\0tpegData,length\0"
    "sendTPEGData(QByteArray,int)\0"
    "reqTPEGDataOff()\0reqTPEGDataOn()\0"
};

void DABTpegIf::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DABTpegIf *_t = static_cast<DABTpegIf *>(_o);
        switch (_id) {
        case 0: _t->sendTPEGData((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->reqTPEGDataOff(); break;
        case 2: _t->reqTPEGDataOn(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DABTpegIf::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DABTpegIf::staticMetaObject = {
    { &QDBusAbstractAdaptor::staticMetaObject, qt_meta_stringdata_DABTpegIf,
      qt_meta_data_DABTpegIf, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DABTpegIf::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DABTpegIf::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DABTpegIf::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DABTpegIf))
        return static_cast<void*>(const_cast< DABTpegIf*>(this));
    return QDBusAbstractAdaptor::qt_metacast(_clname);
}

int DABTpegIf::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void DABTpegIf::sendTPEGData(const QByteArray & _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
