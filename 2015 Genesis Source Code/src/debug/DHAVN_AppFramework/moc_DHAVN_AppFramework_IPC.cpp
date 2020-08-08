/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppFramework_IPC.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppFramework_IPC.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppFramework_IPC.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppFrameworkIPC[] = {

 // content:
       6,       // revision
       0,       // classname
       1,   14, // classinfo
       3,   16, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // classinfo: key, value
      38,   16,

 // signals: signature, parameters, type, tag, flags
      67,   55,   54,   54, 0x05,

 // slots: signature, parameters, type, tag, flags
      93,   55,   54,   54, 0x0a,
     117,   55,   54,   54, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_AppFrameworkIPC[] = {
    "AppFrameworkIPC\0com.lge.car.afw.iface\0"
    "D-Bus Interface\0\0aArg1,aArg2\0"
    "BroadcastEvent(Event,int)\0"
    "ReceiveEvent(Event,int)\0Registered(int,int)\0"
};

void AppFrameworkIPC::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppFrameworkIPC *_t = static_cast<AppFrameworkIPC *>(_o);
        switch (_id) {
        case 0: _t->BroadcastEvent((*reinterpret_cast< const Event(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2]))); break;
        case 1: _t->ReceiveEvent((*reinterpret_cast< const Event(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2]))); break;
        case 2: _t->Registered((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppFrameworkIPC::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppFrameworkIPC::staticMetaObject = {
    { &QDBusAbstractAdaptor::staticMetaObject, qt_meta_stringdata_AppFrameworkIPC,
      qt_meta_data_AppFrameworkIPC, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppFrameworkIPC::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppFrameworkIPC::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppFrameworkIPC::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppFrameworkIPC))
        return static_cast<void*>(const_cast< AppFrameworkIPC*>(this));
    return QDBusAbstractAdaptor::qt_metacast(_clname);
}

int AppFrameworkIPC::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void AppFrameworkIPC::BroadcastEvent(const Event & _t1, const int & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
