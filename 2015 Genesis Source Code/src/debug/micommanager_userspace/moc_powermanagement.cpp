/****************************************************************************
** Meta object code from reading C++ file 'powermanagement.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "include/objects/powermanagement.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'powermanagement.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PowerManagement[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   17,   16,   16, 0x05,
      50,   17,   16,   16, 0x05,
      83,   17,   16,   16, 0x05,
     123,   16,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
     153,   17,   16,   16, 0x0a,
     195,   16,   16,   16, 0x0a,
     221,   17,   16,   16, 0x0a,
     251,   17,   16,   16, 0x0a,
     287,   17,   16,   16, 0x0a,
     325,   17,   16,   16, 0x0a,
     364,   17,   16,   16, 0x0a,
     404,   17,   16,   16, 0x0a,
     456,   17,  445,   16, 0x0a,
     498,   17,  445,   16, 0x0a,
     532,   17,  445,   16, 0x0a,
     566,   16,  445,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_PowerManagement[] = {
    "PowerManagement\0\0in0\0LgeDBus_EventPowerKey(uchar)\0"
    "LgeDBus_EventRequestSleep(uchar)\0"
    "LgeDBus_MPAutoModeTestEvent(QByteArray)\0"
    "LgeDBus_MPAutoModeTestStart()\0"
    "LgeDBus_SetHardwarePowerKeyControl(uchar)\0"
    "LgeDBus_SetSleepConfirm()\0"
    "LgeDBus_BTPowerControl(uchar)\0"
    "LgeDBus_HWCameraPowerControl(uchar)\0"
    "LgeDBus_MPAutoModeTestMsg(QByteArray)\0"
    "LgeDBus_MPAutoTestModeCtrl(QByteArray)\0"
    "LgeDBus_MPAutoTestModeEvent(QByteArray)\0"
    "LgeDBus_AutoTestWIPIDControl(QByteArray)\0"
    "QByteArray\0LgeDBus_MPAutoTestModeRequest(QByteArray)\0"
    "LgeDBus_SetMPModeInfo(QByteArray)\0"
    "LgeDBus_AskMPModeInfo(QByteArray)\0"
    "LgeDBus_AskAutoTestWIPID()\0"
};

void PowerManagement::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PowerManagement *_t = static_cast<PowerManagement *>(_o);
        switch (_id) {
        case 0: _t->LgeDBus_EventPowerKey((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 1: _t->LgeDBus_EventRequestSleep((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 2: _t->LgeDBus_MPAutoModeTestEvent((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 3: _t->LgeDBus_MPAutoModeTestStart(); break;
        case 4: _t->LgeDBus_SetHardwarePowerKeyControl((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 5: _t->LgeDBus_SetSleepConfirm(); break;
        case 6: _t->LgeDBus_BTPowerControl((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 7: _t->LgeDBus_HWCameraPowerControl((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 8: _t->LgeDBus_MPAutoModeTestMsg((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 9: _t->LgeDBus_MPAutoTestModeCtrl((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 10: _t->LgeDBus_MPAutoTestModeEvent((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 11: _t->LgeDBus_AutoTestWIPIDControl((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 12: { QByteArray _r = _t->LgeDBus_MPAutoTestModeRequest((*reinterpret_cast< const QByteArray(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 13: { QByteArray _r = _t->LgeDBus_SetMPModeInfo((*reinterpret_cast< const QByteArray(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 14: { QByteArray _r = _t->LgeDBus_AskMPModeInfo((*reinterpret_cast< const QByteArray(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 15: { QByteArray _r = _t->LgeDBus_AskAutoTestWIPID();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData PowerManagement::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PowerManagement::staticMetaObject = {
    { &ObjectAbstractInterface::staticMetaObject, qt_meta_stringdata_PowerManagement,
      qt_meta_data_PowerManagement, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PowerManagement::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PowerManagement::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PowerManagement::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PowerManagement))
        return static_cast<void*>(const_cast< PowerManagement*>(this));
    if (!strcmp(_clname, "QDBusContext"))
        return static_cast< QDBusContext*>(const_cast< PowerManagement*>(this));
    return ObjectAbstractInterface::qt_metacast(_clname);
}

int PowerManagement::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ObjectAbstractInterface::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void PowerManagement::LgeDBus_EventPowerKey(uchar _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PowerManagement::LgeDBus_EventRequestSleep(uchar _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void PowerManagement::LgeDBus_MPAutoModeTestEvent(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void PowerManagement::LgeDBus_MPAutoModeTestStart()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
