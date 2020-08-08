/****************************************************************************
** Meta object code from reading C++ file 'test_ibox_sender.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/statusbar/listener/test_ibox_sender.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'test_ibox_sender.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TestIBoxSender[] = {

 // content:
       6,       // revision
       0,       // classname
       1,   14, // classinfo
       2,   16, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // classinfo: key, value
      40,   15,

 // signals: signature, parameters, type, tag, flags
      63,   57,   56,   56, 0x05,

 // slots: signature, parameters, type, tag, flags
     117,  113,   56,   56, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_TestIBoxSender[] = {
    "TestIBoxSender\0com.lge.car.iboxtest.osd\0"
    "D-Bus Interface\0\0,,,,,\0"
    "msgArrived(uchar,uchar,uchar,uchar,uchar,QString)\0"
    "msg\0displayOSD(QDBusMessage)\0"
};

void TestIBoxSender::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        TestIBoxSender *_t = static_cast<TestIBoxSender *>(_o);
        switch (_id) {
        case 0: _t->msgArrived((*reinterpret_cast< uchar(*)>(_a[1])),(*reinterpret_cast< uchar(*)>(_a[2])),(*reinterpret_cast< uchar(*)>(_a[3])),(*reinterpret_cast< uchar(*)>(_a[4])),(*reinterpret_cast< uchar(*)>(_a[5])),(*reinterpret_cast< QString(*)>(_a[6]))); break;
        case 1: _t->displayOSD((*reinterpret_cast< const QDBusMessage(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData TestIBoxSender::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TestIBoxSender::staticMetaObject = {
    { &QDBusAbstractAdaptor::staticMetaObject, qt_meta_stringdata_TestIBoxSender,
      qt_meta_data_TestIBoxSender, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TestIBoxSender::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TestIBoxSender::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TestIBoxSender::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TestIBoxSender))
        return static_cast<void*>(const_cast< TestIBoxSender*>(this));
    return QDBusAbstractAdaptor::qt_metacast(_clname);
}

int TestIBoxSender::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractAdaptor::qt_metacall(_c, _id, _a);
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
void TestIBoxSender::msgArrived(uchar _t1, uchar _t2, uchar _t3, uchar _t4, uchar _t5, QString _t6)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
