/****************************************************************************
** Meta object code from reading C++ file 'powerstatuslistener.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/powerstatuslistener.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'powerstatuslistener.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PowerStatusListener[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   20,   20,   20, 0x05,

 // slots: signature, parameters, type, tag, flags
      50,   42,   20,   20, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_PowerStatusListener[] = {
    "PowerStatusListener\0\0powerStatusChanged()\0"
    "message\0PowerStatusEvent(QDBusMessage)\0"
};

void PowerStatusListener::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PowerStatusListener *_t = static_cast<PowerStatusListener *>(_o);
        switch (_id) {
        case 0: _t->powerStatusChanged(); break;
        case 1: _t->PowerStatusEvent((*reinterpret_cast< QDBusMessage(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData PowerStatusListener::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PowerStatusListener::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_PowerStatusListener,
      qt_meta_data_PowerStatusListener, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PowerStatusListener::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PowerStatusListener::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PowerStatusListener::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PowerStatusListener))
        return static_cast<void*>(const_cast< PowerStatusListener*>(this));
    return QObject::qt_metacast(_clname);
}

int PowerStatusListener::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void PowerStatusListener::powerStatusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
