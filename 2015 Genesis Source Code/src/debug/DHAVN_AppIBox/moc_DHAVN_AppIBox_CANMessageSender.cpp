/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppIBox_CANMessageSender.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppIBox_CANMessageSender.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppIBox_CANMessageSender.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CANMessageSender[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      27,   18,   17,   17, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_CANMessageSender[] = {
    "CANMessageSender\0\0canframe\0"
    "Can_data_send(SQCanFrame)\0"
};

void CANMessageSender::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CANMessageSender *_t = static_cast<CANMessageSender *>(_o);
        switch (_id) {
        case 0: _t->Can_data_send((*reinterpret_cast< const SQCanFrame(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CANMessageSender::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CANMessageSender::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CANMessageSender,
      qt_meta_data_CANMessageSender, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CANMessageSender::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CANMessageSender::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CANMessageSender::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CANMessageSender))
        return static_cast<void*>(const_cast< CANMessageSender*>(this));
    return QObject::qt_metacast(_clname);
}

int CANMessageSender::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void CANMessageSender::Can_data_send(const SQCanFrame & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
