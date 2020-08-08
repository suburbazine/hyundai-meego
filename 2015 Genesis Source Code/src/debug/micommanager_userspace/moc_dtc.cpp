/****************************************************************************
** Meta object code from reading C++ file 'dtc.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "include/objects/dtc.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dtc.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DTC[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
       9,    5,    4,    4, 0x05,
      50,    5,    4,    4, 0x05,
      92,    5,    4,    4, 0x05,
     132,    5,    4,    4, 0x05,
     161,    5,    4,    4, 0x05,
     194,    4,    4,    4, 0x05,

 // slots: signature, parameters, type, tag, flags
     227,    5,    4,    4, 0x0a,
     271,    4,  260,    4, 0x0a,
     300,    5,    4,    4, 0x0a,
     339,    4,    4,    4, 0x0a,
     366,    4,    4,    4, 0x0a,
     391,    5,    4,    4, 0x0a,
     422,    5,    4,    4, 0x0a,
     457,    5,    4,    4, 0x0a,
     488,    5,    4,    4, 0x0a,
     521,    5,    4,    4, 0x0a,
     557,    4,    4,    4, 0x0a,
     580,    5,    4,    4, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DTC[] = {
    "DTC\0\0in0\0LgeDBus_EventDTCReadResponse(QByteArray)\0"
    "LgeDBus_EventDTCClearResponse(QByteArray)\0"
    "LgeDBus_EventCANTPMicomSend(QByteArray)\0"
    "LgeDBus_EventEDT(QByteArray)\0"
    "LgeDBus_AuxMicStatusEvent(uchar)\0"
    "LgeDBus_NaviSCCMsgTriggerEvent()\0"
    "LgeDBus_CANTPSendMsg(QByteArray)\0"
    "QByteArray\0LgeDBus_AskMOST_RBDRequest()\0"
    "LgeDBus_AutoTestKeyControl(QByteArray)\0"
    "LgeDBus_MOSTResetRequest()\0"
    "LgeDBus_CANTPMicomRecv()\0"
    "LgeDBus_DTCClearRequest(uchar)\0"
    "LgeDBus_DTCReadRequest(QByteArray)\0"
    "LgeDBus_DTCSendMsg(QByteArray)\0"
    "LgeDBus_AuxMicCompleteMsg(uchar)\0"
    "LgeDBus_NaviSCCDateSend(QByteArray)\0"
    "LgeDBus_TouchCleanup()\0"
    "LgeDBus_KeyTouchRandomTest(QByteArray)\0"
};

void DTC::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DTC *_t = static_cast<DTC *>(_o);
        switch (_id) {
        case 0: _t->LgeDBus_EventDTCReadResponse((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 1: _t->LgeDBus_EventDTCClearResponse((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 2: _t->LgeDBus_EventCANTPMicomSend((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 3: _t->LgeDBus_EventEDT((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 4: _t->LgeDBus_AuxMicStatusEvent((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 5: _t->LgeDBus_NaviSCCMsgTriggerEvent(); break;
        case 6: _t->LgeDBus_CANTPSendMsg((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 7: { QByteArray _r = _t->LgeDBus_AskMOST_RBDRequest();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 8: _t->LgeDBus_AutoTestKeyControl((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 9: _t->LgeDBus_MOSTResetRequest(); break;
        case 10: _t->LgeDBus_CANTPMicomRecv(); break;
        case 11: _t->LgeDBus_DTCClearRequest((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 12: _t->LgeDBus_DTCReadRequest((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 13: _t->LgeDBus_DTCSendMsg((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 14: _t->LgeDBus_AuxMicCompleteMsg((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 15: _t->LgeDBus_NaviSCCDateSend((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 16: _t->LgeDBus_TouchCleanup(); break;
        case 17: _t->LgeDBus_KeyTouchRandomTest((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DTC::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DTC::staticMetaObject = {
    { &ObjectAbstractInterface::staticMetaObject, qt_meta_stringdata_DTC,
      qt_meta_data_DTC, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DTC::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DTC::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DTC::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DTC))
        return static_cast<void*>(const_cast< DTC*>(this));
    if (!strcmp(_clname, "QDBusContext"))
        return static_cast< QDBusContext*>(const_cast< DTC*>(this));
    return ObjectAbstractInterface::qt_metacast(_clname);
}

int DTC::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ObjectAbstractInterface::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void DTC::LgeDBus_EventDTCReadResponse(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DTC::LgeDBus_EventDTCClearResponse(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DTC::LgeDBus_EventCANTPMicomSend(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DTC::LgeDBus_EventEDT(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void DTC::LgeDBus_AuxMicStatusEvent(uchar _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void DTC::LgeDBus_NaviSCCMsgTriggerEvent()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}
QT_END_MOC_NAMESPACE
