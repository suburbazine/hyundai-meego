/****************************************************************************
** Meta object code from reading C++ file 'DABtoFMIf.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DABtoFMIf.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DABtoFMIf.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DABtoFMIf[] = {

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
      15,   11,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      80,   77,   57,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DABtoFMIf[] = {
    "DABtoFMIf\0\0out\0LgeDBus_EventDABPISensitivity(QByteArray)\0"
    "QDBusPendingReply<>\0in\0"
    "LgeDBus_DABPICodeRequest(QByteArray)\0"
};

void DABtoFMIf::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DABtoFMIf *_t = static_cast<DABtoFMIf *>(_o);
        switch (_id) {
        case 0: _t->LgeDBus_EventDABPISensitivity((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 1: { QDBusPendingReply<> _r = _t->LgeDBus_DABPICodeRequest((*reinterpret_cast< const QByteArray(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DABtoFMIf::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DABtoFMIf::staticMetaObject = {
    { &QDBusAbstractInterface::staticMetaObject, qt_meta_stringdata_DABtoFMIf,
      qt_meta_data_DABtoFMIf, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DABtoFMIf::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DABtoFMIf::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DABtoFMIf::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DABtoFMIf))
        return static_cast<void*>(const_cast< DABtoFMIf*>(this));
    return QDBusAbstractInterface::qt_metacast(_clname);
}

int DABtoFMIf::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractInterface::qt_metacall(_c, _id, _a);
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
void DABtoFMIf::LgeDBus_EventDABPISensitivity(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
