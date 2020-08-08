/****************************************************************************
** Meta object code from reading C++ file 'msc_filetransfer.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/modules/most/msc/msc_filetransfer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'msc_filetransfer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MscFileTransfer[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   17,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      47,   16,   16,   16, 0x08,
      81,   16,   16,   16, 0x08,
     115,   16,   16,   16, 0x08,
     149,   16,   16,   16, 0x08,
     176,   16,   16,   16, 0x08,
     211,  205,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MscFileTransfer[] = {
    "MscFileTransfer\0\0,\0transfered(quint64,quint64)\0"
    "PrepareFileTransfer(QDBusVariant)\0"
    "RequestTransferExit(QDBusVariant)\0"
    "CheckActivateModule(QDBusVariant)\0"
    "TransferData(QDBusVariant)\0"
    "TimeoutRequestTransferExit()\0state\0"
    "iBoxBlockVisibleSlot(bool)\0"
};

void MscFileTransfer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MscFileTransfer *_t = static_cast<MscFileTransfer *>(_o);
        switch (_id) {
        case 0: _t->transfered((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< quint64(*)>(_a[2]))); break;
        case 1: _t->PrepareFileTransfer((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 2: _t->RequestTransferExit((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 3: _t->CheckActivateModule((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 4: _t->TransferData((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 5: _t->TimeoutRequestTransferExit(); break;
        case 6: _t->iBoxBlockVisibleSlot((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MscFileTransfer::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MscFileTransfer::staticMetaObject = {
    { &MscAbstract::staticMetaObject, qt_meta_stringdata_MscFileTransfer,
      qt_meta_data_MscFileTransfer, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MscFileTransfer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MscFileTransfer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MscFileTransfer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MscFileTransfer))
        return static_cast<void*>(const_cast< MscFileTransfer*>(this));
    return MscAbstract::qt_metacast(_clname);
}

int MscFileTransfer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MscAbstract::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void MscFileTransfer::transfered(quint64 _t1, quint64 _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
