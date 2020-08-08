/****************************************************************************
** Meta object code from reading C++ file 'inicversion.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/system/firmware/inicversion.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'inicversion.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_InicVersion[] = {

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
      13,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      27,   12,   12,   12, 0x08,
      59,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_InicVersion[] = {
    "InicVersion\0\0dataArrived()\0"
    "inicVersionReceived(QByteArray)\0"
    "inicVersionError()\0"
};

void InicVersion::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        InicVersion *_t = static_cast<InicVersion *>(_o);
        switch (_id) {
        case 0: _t->dataArrived(); break;
        case 1: _t->inicVersionReceived((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 2: _t->inicVersionError(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData InicVersion::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject InicVersion::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_InicVersion,
      qt_meta_data_InicVersion, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &InicVersion::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *InicVersion::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *InicVersion::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_InicVersion))
        return static_cast<void*>(const_cast< InicVersion*>(this));
    return QObject::qt_metacast(_clname);
}

int InicVersion::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void InicVersion::dataArrived()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
