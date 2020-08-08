/****************************************************************************
** Meta object code from reading C++ file 'spi_comm_thread.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/comm_interface/spi_comm_thread.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'spi_comm_thread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CSpiCommThread[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,
      40,   15,   15,   15, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_CSpiCommThread[] = {
    "CSpiCommThread\0\0dataArrived(QByteArray)\0"
    "printDebug(QByteArray)\0"
};

void CSpiCommThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CSpiCommThread *_t = static_cast<CSpiCommThread *>(_o);
        switch (_id) {
        case 0: _t->dataArrived((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 1: _t->printDebug((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CSpiCommThread::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CSpiCommThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_CSpiCommThread,
      qt_meta_data_CSpiCommThread, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CSpiCommThread::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CSpiCommThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CSpiCommThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CSpiCommThread))
        return static_cast<void*>(const_cast< CSpiCommThread*>(this));
    return QThread::qt_metacast(_clname);
}

int CSpiCommThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
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
void CSpiCommThread::dataArrived(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CSpiCommThread::printDebug(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
