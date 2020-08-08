/****************************************************************************
** Meta object code from reading C++ file 'dtc_adaptor.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "include/adaptors/dtc_adaptor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dtc_adaptor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DTCAdaptor[] = {

 // content:
       6,       // revision
       0,       // classname
       2,   14, // classinfo
      18,   18, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // classinfo: key, value
      33,   11,
    2510,   49,

 // signals: signature, parameters, type, tag, flags
    2535, 2531, 2530, 2530, 0x05,
    2568, 2531, 2530, 2530, 0x05,
    2608, 2531, 2530, 2530, 0x05,
    2650, 2531, 2530, 2530, 0x05,
    2691, 2531, 2530, 2530, 0x05,
    2720, 2530, 2530, 2530, 0x05,

 // slots: signature, parameters, type, tag, flags
    2764, 2530, 2753, 2530, 0x0a,
    2793, 2531, 2530, 2530, 0x0a,
    2832, 2531, 2530, 2530, 0x0a,
    2865, 2530, 2530, 2530, 0x0a,
    2890, 2531, 2530, 2530, 0x0a,
    2923, 2531, 2530, 2530, 0x0a,
    2954, 2531, 2530, 2530, 0x0a,
    2989, 2531, 2530, 2530, 0x0a,
    3020, 2531, 2530, 2530, 0x0a,
    3059, 2530, 2530, 2530, 0x0a,
    3086, 2531, 2530, 2530, 0x0a,
    3122, 2530, 2530, 2530, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DTCAdaptor[] = {
    "DTCAdaptor\0com.lge.car.micom.dtc\0"
    "D-Bus Interface\0"
    "  <interface name=\"com.lge.car.micom.dtc\">\n    <method name=\"LgeDB"
    "us_DTCSendMsg\">\n      <!-- 0x018A 20120325(in:4byte) -->\n      <arg"
    " direction=\"in\" type=\"ay\"/>\n    </method>\n    <method name=\"Lge"
    "DBus_TouchCleanup\"/>\n    <method name=\"LgeDBus_DTCReadRequest\">\n "
    "     <!-- 0x028A 20120325(in:2bytes) -->\n      <!-- 0x828A 20120325(i"
    "n:variable) -->\n      <arg direction=\"in\" type=\"ay\"/>\n    </meth"
    "od>\n    <method name=\"LgeDBus_DTCClearRequest\">\n      <!-- 0x028B "
    "20120325(in:1bytes) -->\n      <!-- 0x828B 20120325(in:2byte) -->\n   "
    "   <arg direction=\"in\" type=\"y\"/>\n    </method>\n    <method name"
    "=\"LgeDBus_AskMOST_RBDRequest\">\n      <!-- 0x0261 20120510 -->\n    "
    "  <!-- RECV : 0x8261 20120510(in:2byte) -->\n      <arg direction=\"ou"
    "t\" type=\"ay\"/>\n    </method>\n    <method name=\"LgeDBus_MOSTReset"
    "Request\"/>\n    <!-- 0x0262 20120518 -->\n    <method name=\"LgeDBus_"
    "CANTPSendMsg\">\n      <!-- 0x0189 20120403(in:variable) -->\n      <!"
    "-- RECV : 0x8389 20120403(in:variable) -->\n      <arg direction=\"in\""
    " type=\"ay\"/>\n    </method>\n    <method name=\"LgeDBus_AutoTestKeyC"
    "ontrol\">\n      <!-- 0x0171 20120919(in:variable) -->\n      <arg dir"
    "ection=\"in\" type=\"ay\"/>\n    </method>\n    <method name=\"LgeDBus"
    "_CANTPMicomRecv\"/>\n    <!-- 0x0403 20120514(in:11byte) -->\n    <met"
    "hod name=\"LgeDBus_AuxMicCompleteMsg\">\n      <!-- 0x018B 20121220(in"
    ":1bytes) -->\n      <arg direction=\"in\" type=\"y\"/>\n    </method>\n"
    "    <method name=\"LgeDBus_NaviSCCDateSend\">\n      <!-- 0x0199 20130"
    "110 -->\n      <arg direction=\"in\" type=\"ay\"/>\n    </method>\n   "
    " <method name=\"LgeDBus_KeyTouchRandomTest\">\n      <!-- 0x0665 20130"
    "611 -->\n      <arg direction=\"in\" type=\"ay\"/>\n    </method>\n   "
    " <signal name=\"LgeDBus_NaviSCCMsgTriggerEvent\"/>\n    <!-- 0x8396 fr"
    "om micom -->\n    <signal name=\"LgeDBus_EventDTCClearResponse\">\n   "
    "   <!-- 0x828B from micom (out:variable)-->\n      <arg direction=\"ou"
    "t\" type=\"ay\"/>\n    </signal>\n    <signal name=\"LgeDBus_EventDTCR"
    "eadResponse\">\n      <!-- 0x828A from micom (out:variable)-->\n      "
    "<arg direction=\"out\" type=\"ay\"/>\n    </signal>\n    <signal name="
    "\"LgeDBus_EventCANTPMicomSend\">\n      <!-- 0x8404 from micom (out:11"
    "byte)-->\n      <arg direction=\"out\" type=\"ay\"/>\n    </signal>\n "
    "   <signal name=\"LgeDBus_EventEDT\">\n      <!-- 0x8376 from micom (o"
    "ut:1byte)-->\n      <arg direction=\"out\" type=\"ay\"/>\n    </signal"
    ">\n    <signal name=\"LgeDBus_AuxMicStatusEvent\">\n      <!-- 0x8377 "
    "from micom (out:1byte)-->\n      <arg direction=\"out\" type=\"y\"/>\n"
    "    </signal>\n  </interface>\n\0"
    "D-Bus Introspection\0\0in0\0"
    "LgeDBus_AuxMicStatusEvent(uchar)\0"
    "LgeDBus_EventCANTPMicomSend(QByteArray)\0"
    "LgeDBus_EventDTCClearResponse(QByteArray)\0"
    "LgeDBus_EventDTCReadResponse(QByteArray)\0"
    "LgeDBus_EventEDT(QByteArray)\0"
    "LgeDBus_NaviSCCMsgTriggerEvent()\0"
    "QByteArray\0LgeDBus_AskMOST_RBDRequest()\0"
    "LgeDBus_AutoTestKeyControl(QByteArray)\0"
    "LgeDBus_AuxMicCompleteMsg(uchar)\0"
    "LgeDBus_CANTPMicomRecv()\0"
    "LgeDBus_CANTPSendMsg(QByteArray)\0"
    "LgeDBus_DTCClearRequest(uchar)\0"
    "LgeDBus_DTCReadRequest(QByteArray)\0"
    "LgeDBus_DTCSendMsg(QByteArray)\0"
    "LgeDBus_KeyTouchRandomTest(QByteArray)\0"
    "LgeDBus_MOSTResetRequest()\0"
    "LgeDBus_NaviSCCDateSend(QByteArray)\0"
    "LgeDBus_TouchCleanup()\0"
};

void DTCAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DTCAdaptor *_t = static_cast<DTCAdaptor *>(_o);
        switch (_id) {
        case 0: _t->LgeDBus_AuxMicStatusEvent((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 1: _t->LgeDBus_EventCANTPMicomSend((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 2: _t->LgeDBus_EventDTCClearResponse((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 3: _t->LgeDBus_EventDTCReadResponse((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 4: _t->LgeDBus_EventEDT((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 5: _t->LgeDBus_NaviSCCMsgTriggerEvent(); break;
        case 6: { QByteArray _r = _t->LgeDBus_AskMOST_RBDRequest();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 7: _t->LgeDBus_AutoTestKeyControl((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 8: _t->LgeDBus_AuxMicCompleteMsg((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 9: _t->LgeDBus_CANTPMicomRecv(); break;
        case 10: _t->LgeDBus_CANTPSendMsg((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 11: _t->LgeDBus_DTCClearRequest((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 12: _t->LgeDBus_DTCReadRequest((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 13: _t->LgeDBus_DTCSendMsg((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 14: _t->LgeDBus_KeyTouchRandomTest((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 15: _t->LgeDBus_MOSTResetRequest(); break;
        case 16: _t->LgeDBus_NaviSCCDateSend((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 17: _t->LgeDBus_TouchCleanup(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DTCAdaptor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DTCAdaptor::staticMetaObject = {
    { &QDBusAbstractAdaptor::staticMetaObject, qt_meta_stringdata_DTCAdaptor,
      qt_meta_data_DTCAdaptor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DTCAdaptor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DTCAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DTCAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DTCAdaptor))
        return static_cast<void*>(const_cast< DTCAdaptor*>(this));
    return QDBusAbstractAdaptor::qt_metacast(_clname);
}

int DTCAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractAdaptor::qt_metacall(_c, _id, _a);
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
void DTCAdaptor::LgeDBus_AuxMicStatusEvent(uchar _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DTCAdaptor::LgeDBus_EventCANTPMicomSend(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DTCAdaptor::LgeDBus_EventDTCClearResponse(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DTCAdaptor::LgeDBus_EventDTCReadResponse(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void DTCAdaptor::LgeDBus_EventEDT(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void DTCAdaptor::LgeDBus_NaviSCCMsgTriggerEvent()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}
QT_END_MOC_NAMESPACE
