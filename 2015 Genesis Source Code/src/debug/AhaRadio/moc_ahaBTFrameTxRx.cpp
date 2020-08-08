/****************************************************************************
** Meta object code from reading C++ file 'ahaBTFrameTxRx.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/ahaBTFrameTxRx.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ahaBTFrameTxRx.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CAhaBTFrameTxRx[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      25,   17,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      60,   50,   16,   16, 0x0a,
      94,   81,   16,   16, 0x0a,
     118,   16,   16,   16, 0x0a,
     158,  152,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CAhaBTFrameTxRx[] = {
    "CAhaBTFrameTxRx\0\0inFrame\0"
    "DataReceived(QByteArray)\0inConnect\0"
    "onSPPConnected(bool)\0inDisconnect\0"
    "onSPPDisconnected(bool)\0"
    "onSPPDataReadComplete(QByteArray)\0"
    "error\0onSPPDataWriteToBTStatus(int)\0"
};

void CAhaBTFrameTxRx::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CAhaBTFrameTxRx *_t = static_cast<CAhaBTFrameTxRx *>(_o);
        switch (_id) {
        case 0: _t->DataReceived((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 1: _t->onSPPConnected((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->onSPPDisconnected((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->onSPPDataReadComplete((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 4: _t->onSPPDataWriteToBTStatus((*reinterpret_cast< const int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CAhaBTFrameTxRx::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CAhaBTFrameTxRx::staticMetaObject = {
    { &CAhaAbsFrameTxRx::staticMetaObject, qt_meta_stringdata_CAhaBTFrameTxRx,
      qt_meta_data_CAhaBTFrameTxRx, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CAhaBTFrameTxRx::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CAhaBTFrameTxRx::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CAhaBTFrameTxRx::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CAhaBTFrameTxRx))
        return static_cast<void*>(const_cast< CAhaBTFrameTxRx*>(this));
    return CAhaAbsFrameTxRx::qt_metacast(_clname);
}

int CAhaBTFrameTxRx::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CAhaAbsFrameTxRx::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void CAhaBTFrameTxRx::DataReceived(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
