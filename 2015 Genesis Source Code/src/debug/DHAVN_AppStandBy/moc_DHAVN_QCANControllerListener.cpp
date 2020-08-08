/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_QCANControllerListener.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_QCANControllerListener.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_QCANControllerListener.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QCANControllerListener[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      34,   24,   23,   23, 0x05,
      74,   65,   23,   23, 0x05,

 // slots: signature, parameters, type, tag, flags
     100,   23,   23,   23, 0x0a,
     136,   23,   23,   23, 0x0a,
     149,   23,   23,   23, 0x0a,
     176,   23,   23,   23, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QCANControllerListener[] = {
    "QCANControllerListener\0\0canIdList\0"
    "Can_filter_msg_id(SQCanIdList)\0canframe\0"
    "Can_data_send(SQCanFrame)\0"
    "CQCANControllerTest_Filter_can_id()\0"
    "updateTime()\0onSettingsModechange(bool)\0"
    "onAccStatus(int)\0"
};

void QCANControllerListener::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QCANControllerListener *_t = static_cast<QCANControllerListener *>(_o);
        switch (_id) {
        case 0: _t->Can_filter_msg_id((*reinterpret_cast< const SQCanIdList(*)>(_a[1]))); break;
        case 1: _t->Can_data_send((*reinterpret_cast< const SQCanFrame(*)>(_a[1]))); break;
        case 2: _t->CQCANControllerTest_Filter_can_id(); break;
        case 3: _t->updateTime(); break;
        case 4: _t->onSettingsModechange((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->onAccStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QCANControllerListener::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QCANControllerListener::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QCANControllerListener,
      qt_meta_data_QCANControllerListener, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QCANControllerListener::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QCANControllerListener::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QCANControllerListener::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QCANControllerListener))
        return static_cast<void*>(const_cast< QCANControllerListener*>(this));
    return QObject::qt_metacast(_clname);
}

int QCANControllerListener::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void QCANControllerListener::Can_filter_msg_id(const SQCanIdList & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QCANControllerListener::Can_data_send(const SQCanFrame & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
