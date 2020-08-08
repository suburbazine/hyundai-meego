/****************************************************************************
** Meta object code from reading C++ file 'dab.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "include/objects/dab.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dab.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DAB[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
       9,    5,    4,    4, 0x05,

 // slots: signature, parameters, type, tag, flags
      51,    5,    4,    4, 0x0a,
      88,    5,    4,    4, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DAB[] = {
    "DAB\0\0in0\0LgeDBus_EventDABPISensitivity(QByteArray)\0"
    "LgeDBus_DABPICodeRequest(QByteArray)\0"
    "LgeDBus_DABTAStatus(uchar)\0"
};

void DAB::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DAB *_t = static_cast<DAB *>(_o);
        switch (_id) {
        case 0: _t->LgeDBus_EventDABPISensitivity((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 1: _t->LgeDBus_DABPICodeRequest((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 2: _t->LgeDBus_DABTAStatus((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DAB::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DAB::staticMetaObject = {
    { &ObjectAbstractInterface::staticMetaObject, qt_meta_stringdata_DAB,
      qt_meta_data_DAB, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DAB::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DAB::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DAB::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DAB))
        return static_cast<void*>(const_cast< DAB*>(this));
    if (!strcmp(_clname, "QDBusContext"))
        return static_cast< QDBusContext*>(const_cast< DAB*>(this));
    return ObjectAbstractInterface::qt_metacast(_clname);
}

int DAB::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ObjectAbstractInterface::qt_metacall(_c, _id, _a);
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
void DAB::LgeDBus_EventDABPISensitivity(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
