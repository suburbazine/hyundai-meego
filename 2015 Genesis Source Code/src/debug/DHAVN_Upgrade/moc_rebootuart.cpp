/****************************************************************************
** Meta object code from reading C++ file 'rebootuart.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/util/rebootuart.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'rebootuart.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RebootUART[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      37,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_RebootUART[] = {
    "RebootUART\0\0dataReceived(QByteArray)\0"
    "timeoutUART()\0"
};

void RebootUART::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        RebootUART *_t = static_cast<RebootUART *>(_o);
        switch (_id) {
        case 0: _t->dataReceived((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 1: _t->timeoutUART(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData RebootUART::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject RebootUART::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_RebootUART,
      qt_meta_data_RebootUART, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RebootUART::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RebootUART::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RebootUART::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RebootUART))
        return static_cast<void*>(const_cast< RebootUART*>(this));
    return QObject::qt_metacast(_clname);
}

int RebootUART::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_END_MOC_NAMESPACE
