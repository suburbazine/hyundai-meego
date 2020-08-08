/****************************************************************************
** Meta object code from reading C++ file 'tab2driver.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/devicemanager/tab2driver.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tab2driver.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Tab2Driver[] = {

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
      12,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      24,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Tab2Driver[] = {
    "Tab2Driver\0\0ReadReady()\0HandlePolling()\0"
};

void Tab2Driver::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Tab2Driver *_t = static_cast<Tab2Driver *>(_o);
        switch (_id) {
        case 0: _t->ReadReady(); break;
        case 1: _t->HandlePolling(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData Tab2Driver::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Tab2Driver::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_Tab2Driver,
      qt_meta_data_Tab2Driver, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Tab2Driver::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Tab2Driver::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Tab2Driver::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Tab2Driver))
        return static_cast<void*>(const_cast< Tab2Driver*>(this));
    return QThread::qt_metacast(_clname);
}

int Tab2Driver::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void Tab2Driver::ReadReady()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
